/*
                Application
                     ↓
               Load Balancer
                     ↓
          -----------------------
          ↓          ↓          ↓
       Server 1   Server 2   Server 3
          \          |          /
           \         |         /
              Redis Cluster
                   ↓
            Consistent Hashing
             /              \
          Shard 1          Shard 2
            ↓                ↓
         Replica          Replica
            ↓
          Database
*/

#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <chrono>

using namespace std;


// ======================================================
// DATABASE
// ======================================================

class Database {

private:

    unordered_map<string, string> data;


public:

    void save(string key, string value) {

        data[key] = value;

        cout << "Database: Saved "
             << key << " -> "
             << value << endl;
    }


    string get(string key) {

        cout << "Database: Fetching "
             << key << endl;

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }
};


// ======================================================
// REDIS NODE
// ======================================================

class RedisNode {

private:

    string name;

    unordered_map<string, string> data;


public:

    RedisNode(string name) {
        this->name = name;
    }


    void set(string key, string value) {

        data[key] = value;

        cout << name
             << " SET "
             << key << " -> "
             << value << endl;
    }


    string get(string key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }


    void remove(string key) {

        data.erase(key);
    }


    string getName() {

        return name;
    }
};


// ======================================================
// REDIS SHARD
// ======================================================

class RedisShard {

private:

    RedisNode* primary;

    RedisNode* replica;


public:

    RedisShard(
        RedisNode* primary,
        RedisNode* replica
    ) {

        this->primary = primary;
        this->replica = replica;
    }


    // Write to primary and replica
    void set(string key, string value) {

        primary->set(key, value);

        // Replication
        replica->set(key, value);
    }


    // Read from primary
    string get(string key) {

        return primary->get(key);
    }


    void remove(string key) {

        primary->remove(key);

        replica->remove(key);
    }


    string getPrimaryName() {

        return primary->getName();
    }
};


// ======================================================
// CONSISTENT HASHING
// ======================================================

class ConsistentHashing {

private:

    map<size_t, RedisShard*> ring;


    size_t hashKey(string key) {

        return hash<string>{}(key);
    }


public:

    void addShard(
        RedisShard* shard,
        string shardName
    ) {

        // Virtual nodes
        for (int i = 0; i < 5; i++) {

            string virtualNode =
                shardName +
                "#VN" +
                to_string(i);

            size_t hashValue =
                hashKey(virtualNode);

            ring[hashValue] = shard;
        }
    }


    RedisShard* getShard(string key) {

        if (ring.empty()) {
            return nullptr;
        }


        size_t hashValue =
            hashKey(key);


        auto it =
            ring.lower_bound(hashValue);


        // Wrap around
        if (it == ring.end()) {

            it = ring.begin();
        }


        return it->second;
    }
};


// ======================================================
// REDIS CLUSTER
// ======================================================

class RedisCluster {

private:

    ConsistentHashing hashing;


public:

    void addShard(
        RedisShard* shard,
        string name
    ) {

        hashing.addShard(shard, name);
    }


    // SET
    void set(string key, string value) {

        RedisShard* shard =
            hashing.getShard(key);


        if (shard == nullptr) {

            cout << "No Redis shard available\n";

            return;
        }


        cout << "\nKey "
             << key
             << " routed to "
             << shard->getPrimaryName()
             << endl;


        shard->set(key, value);
    }


    // GET
    string get(string key) {

        RedisShard* shard =
            hashing.getShard(key);


        if (shard == nullptr) {

            return "";
        }


        cout << "\nKey "
             << key
             << " routed to "
             << shard->getPrimaryName()
             << endl;


        string value =
            shard->get(key);


        if (value == "") {

            cout << "Redis: Cache Miss\n";
        }
        else {

            cout << "Redis: Cache Hit\n";
        }


        return value;
    }


    // DELETE
    void remove(string key) {

        RedisShard* shard =
            hashing.getShard(key);


        if (shard != nullptr) {

            shard->remove(key);
        }
    }
};


// ======================================================
// APPLICATION SERVER
// ======================================================

class ApplicationServer {

private:

    int id;

    RedisCluster* redis;

    Database* db;


public:

    ApplicationServer(
        int id,
        RedisCluster* redis,
        Database* db
    ) {

        this->id = id;
        this->redis = redis;
        this->db = db;
    }


    // ==============================================
    // CACHE-ASIDE READ
    // ==============================================

    string getData(string key) {

        cout << "\nServer "
             << id
             << " received request\n";


        // 1. Check Redis

        string value =
            redis->get(key);


        // Cache Hit

        if (value != "") {

            cout << "Server "
                 << id
                 << ": Returning cached data\n";

            return value;
        }


        // Cache Miss

        cout << "Server "
             << id
             << ": Cache Miss\n";


        // 2. Database

        value =
            db->get(key);


        // Data exists

        if (value != "") {

            // 3. Store in Redis

            redis->set(key, value);
        }


        return value;
    }


    // ==============================================
    // WRITE + CACHE INVALIDATION
    // ==============================================

    void updateData(
        string key,
        string value
    ) {

        cout << "\nServer "
             << id
             << " updating data\n";


        // 1. Update database

        db->save(key, value);


        // 2. Invalidate cache

        redis->remove(key);


        cout << "Redis: Cache invalidated\n";
    }
};


// ======================================================
// LOAD BALANCER
// ======================================================

class LoadBalancer {

private:

    vector<ApplicationServer*> servers;

    int index = 0;


public:

    void addServer(
        ApplicationServer* server
    ) {

        servers.push_back(server);
    }


    ApplicationServer* getServer() {

        if (servers.empty()) {

            return nullptr;
        }


        ApplicationServer* server =
            servers[index];


        index =
            (index + 1) % servers.size();


        return server;
    }
};


// ======================================================
// MAIN
// ======================================================

int main() {


    // --------------------------------------------------
    // Database
    // --------------------------------------------------

    Database db;


    db.save(
        "user:1",
        "Vishal"
    );


    db.save(
        "user:2",
        "Rahul"
    );


    // --------------------------------------------------
    // Redis Nodes
    // --------------------------------------------------

    RedisNode redis1Primary(
        "Redis-1-Primary"
    );

    RedisNode redis1Replica(
        "Redis-1-Replica"
    );


    RedisNode redis2Primary(
        "Redis-2-Primary"
    );

    RedisNode redis2Replica(
        "Redis-2-Replica"
    );


    // --------------------------------------------------
    // Redis Shards
    // --------------------------------------------------

    RedisShard shard1(
        &redis1Primary,
        &redis1Replica
    );


    RedisShard shard2(
        &redis2Primary,
        &redis2Replica
    );


    // --------------------------------------------------
    // Redis Cluster
    // --------------------------------------------------

    RedisCluster redisCluster;


    redisCluster.addShard(
        &shard1,
        "Shard-1"
    );


    redisCluster.addShard(
        &shard2,
        "Shard-2"
    );


    // --------------------------------------------------
    // Application Servers
    // --------------------------------------------------

    ApplicationServer server1(
        1,
        &redisCluster,
        &db
    );


    ApplicationServer server2(
        2,
        &redisCluster,
        &db
    );


    ApplicationServer server3(
        3,
        &redisCluster,
        &db
    );


    // --------------------------------------------------
    // Load Balancer
    // --------------------------------------------------

    LoadBalancer loadBalancer;


    loadBalancer.addServer(
        &server1
    );


    loadBalancer.addServer(
        &server2
    );


    loadBalancer.addServer(
        &server3
    );


    // ==================================================
    // REQUEST 1
    // ==================================================

    cout << "\n==============================";
    cout << "\nREQUEST 1";
    cout << "\n==============================\n";


    ApplicationServer* server =
        loadBalancer.getServer();


    cout << "Result: "
         << server->getData("user:1")
         << endl;


    // ==================================================
    // REQUEST 2
    // ==================================================

    cout << "\n==============================";
    cout << "\nREQUEST 2";
    cout << "\n==============================\n";


    server =
        loadBalancer.getServer();


    cout << "Result: "
         << server->getData("user:1")
         << endl;


    // ==================================================
    // UPDATE
    // ==================================================

    cout << "\n==============================";
    cout << "\nUPDATE";
    cout << "\n==============================\n";


    server =
        loadBalancer.getServer();


    server->updateData(
        "user:1",
        "Amit"
    );


    // ==================================================
    // REQUEST 3
    // ==================================================

    cout << "\n==============================";
    cout << "\nREQUEST 3";
    cout << "\n==============================\n";


    server =
        loadBalancer.getServer();


    cout << "Result: "
         << server->getData("user:1")
         << endl;


    return 0;
}