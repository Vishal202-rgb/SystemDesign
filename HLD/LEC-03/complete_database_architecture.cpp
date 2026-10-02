#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;


// ======================================================
// 1. DATABASE
// ======================================================

class Database {

private:
    unordered_map<int, string> data;

public:

    void insert(int id, string name) {
        data[id] = name;
    }

    string get(int id) {

        if (data.find(id) != data.end()) {
            return data[id];
        }

        return "Not Found";
    }

    void update(int id, string name) {
        data[id] = name;
    }
};


// ======================================================
// 2. REDIS CACHE
// ======================================================

class RedisCache {

private:
    unordered_map<int, string> cache;

public:

    bool contains(int id) {
        return cache.find(id) != cache.end();
    }

    string get(int id) {
        return cache[id];
    }

    void set(int id, string value) {
        cache[id] = value;
    }

    void remove(int id) {
        cache.erase(id);
    }
};


// ======================================================
// 3. BLOOM FILTER
// ======================================================

class BloomFilter {

private:
    vector<bool> bits;
    int size;

    int hashFunction(int id) {
        return id % size;
    }

public:

    BloomFilter(int size) {
        this->size = size;
        bits.resize(size, false);
    }

    void add(int id) {

        int index = hashFunction(id);

        bits[index] = true;
    }

    bool mightContain(int id) {

        int index = hashFunction(id);

        return bits[index];
    }
};


// ======================================================
// 4. DATABASE SHARD
// ======================================================

class DatabaseShard {

private:
    Database database;

public:

    void insert(int id, string name) {
        database.insert(id, name);
    }

    string get(int id) {
        return database.get(id);
    }

    void update(int id, string name) {
        database.update(id, name);
    }
};


// ======================================================
// 5. DATABASE CLUSTER
// ======================================================

class DatabaseCluster {

private:

    vector<DatabaseShard> shards;

    int numberOfShards;

    int getShard(int id) {

        return id % numberOfShards;
    }

public:

    DatabaseCluster(int count) {

        numberOfShards = count;

        for (int i = 0; i < count; i++) {
            shards.emplace_back();
        }
    }


    void insert(int id, string name) {

        int shardId = getShard(id);

        shards[shardId].insert(id, name);
    }


    string get(int id) {

        int shardId = getShard(id);

        return shards[shardId].get(id);
    }


    void update(int id, string name) {

        int shardId = getShard(id);

        shards[shardId].update(id, name);
    }
};


// ======================================================
// 6. APPLICATION SERVER
// ======================================================

class ApplicationServer {

private:

    RedisCache cache;

    BloomFilter bloomFilter;

    DatabaseCluster database;

public:

    ApplicationServer()
        : bloomFilter(100),
          database(3) {
    }


    // Add user
    void addUser(int id, string name) {

        database.insert(id, name);

        bloomFilter.add(id);

        cache.set(id, name);
    }


    // Get user
    string getUser(int id) {

        cout << "\nRequest for User " << id << endl;


        // ---------------------------------
        // Bloom Filter
        // ---------------------------------

        if (!bloomFilter.mightContain(id)) {

            cout << "Bloom Filter: User definitely does not exist.\n";

            return "Not Found";
        }


        cout << "Bloom Filter: User may exist.\n";


        // ---------------------------------
        // Redis Cache
        // ---------------------------------

        if (cache.contains(id)) {

            cout << "Redis: Cache HIT\n";

            return cache.get(id);
        }


        cout << "Redis: Cache MISS\n";


        // ---------------------------------
        // Database
        // ---------------------------------

        string result = database.get(id);

        cout << "Database: Fetching data\n";


        // Store result in cache
        if (result != "Not Found") {
            cache.set(id, result);
        }


        return result;
    }


    // Update user
    void updateUser(int id, string name) {

        cout << "\nUpdating User " << id << endl;


        // Update database
        database.update(id, name);


        // Invalidate old cache
        cache.remove(id);


        cout << "Database updated.\n";
        cout << "Redis cache invalidated.\n";
    }
};


// ======================================================
// 7. LOAD BALANCER
// ======================================================

class LoadBalancer {

private:

    vector<ApplicationServer*> servers;

    int currentServer = 0;

public:

    void addServer(ApplicationServer* server) {
        servers.push_back(server);
    }


    ApplicationServer* getServer() {

        ApplicationServer* server =
            servers[currentServer];

        currentServer =
            (currentServer + 1) % servers.size();

        return server;
    }
};


// ======================================================
// MAIN
// ======================================================

int main() {

    // ---------------------------------
    // Create Application Servers
    // ---------------------------------

    ApplicationServer server1;
    ApplicationServer server2;


    // ---------------------------------
    // Load Balancer
    // ---------------------------------

    LoadBalancer loadBalancer;

    loadBalancer.addServer(&server1);
    loadBalancer.addServer(&server2);


    // ---------------------------------
    // Add users
    // ---------------------------------

    server1.addUser(1, "Vishal");
    server1.addUser(2, "Rahul");
    server1.addUser(3, "Aman");


    // ---------------------------------
    // Request 1
    // ---------------------------------

    ApplicationServer* server =
        loadBalancer.getServer();

    cout << "Result: "
         << server->getUser(1)
         << endl;


    // ---------------------------------
    // Request 2
    // ---------------------------------

    server = loadBalancer.getServer();

    cout << "Result: "
         << server->getUser(2)
         << endl;


    // ---------------------------------
    // Request 3
    // ---------------------------------

    server = loadBalancer.getServer();

    cout << "Result: "
         << server->getUser(1)
         << endl;


    // ---------------------------------
    // Update User
    // ---------------------------------

    server1.updateUser(1, "Vishal Kumar");


    // ---------------------------------
    // Request after update
    // ---------------------------------

    cout << "\nAfter Update:" << endl;

    cout << "Result: "
         << server1.getUser(1)
         << endl;


    return 0;
}

/*
                         Client
                           |
                           v
                    Load Balancer
                     /           \
                    v             v
              App Server 1   App Server 2
                    |
                    v
              Bloom Filter
                    |
                    v
                 Redis
                /     \
             HIT       MISS
              |          |
              v          v
            Return    DB Cluster
                         |
                +--------+--------+
                |        |        |
              Shard 0  Shard 1  Shard 2
*/