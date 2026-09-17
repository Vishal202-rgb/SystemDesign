#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;


// --------------------
// Redis Server
// --------------------
class RedisServer {

private:
    string serverName;
    unordered_map<string, string> data;

public:

    RedisServer(string name) {
        serverName = name;
    }

    void set(string key, string value) {
        data[key] = value;

        cout << serverName << " SET "
             << key << " -> "
             << value << endl;
    }

    string get(string key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }

    string getName() {
        return serverName;
    }
};


// --------------------
// Consistent Hashing
// --------------------
class ConsistentHashing {

private:

    // hash position -> server
    map<size_t, RedisServer*> ring;

    // server name -> server object
    unordered_map<string, RedisServer*> servers;


    size_t hashKey(string key) {
        return hash<string>{}(key);
    }


public:

    void addServer(RedisServer* server) {

        servers[server->getName()] = server;

        // Add multiple virtual nodes
        for (int i = 0; i < 5; i++) {

            string virtualNode =
                server->getName() +
                "#VN" +
                to_string(i);

            size_t hashValue = hashKey(virtualNode);

            ring[hashValue] = server;
        }
    }


    RedisServer* getServer(string key) {

        if (ring.empty()) {
            return nullptr;
        }

        size_t hashValue = hashKey(key);


        // Find first server clockwise
        auto it = ring.lower_bound(hashValue);


        // Wrap around
        if (it == ring.end()) {
            it = ring.begin();
        }


        return it->second;
    }
};


// --------------------
// Redis Cluster
// --------------------
class RedisCluster {

private:

    ConsistentHashing hashing;


public:

    void addServer(RedisServer* server) {

        hashing.addServer(server);
    }


    void set(string key, string value) {

        RedisServer* server =
            hashing.getServer(key);

        if (server == nullptr) {
            cout << "No Redis server available\n";
            return;
        }

        cout << "\nKey " << key
             << " routed to "
             << server->getName() << endl;

        server->set(key, value);
    }


    string get(string key) {

        RedisServer* server =
            hashing.getServer(key);

        if (server == nullptr) {
            return "";
        }

        cout << "\nKey " << key
             << " routed to "
             << server->getName() << endl;

        string value = server->get(key);

        if (value == "") {
            cout << "Cache Miss\n";
        }
        else {
            cout << "Cache Hit\n";
        }

        return value;
    }
};


// --------------------
// Main
// --------------------
int main() {

    RedisServer redis1("Redis-1");
    RedisServer redis2("Redis-2");
    RedisServer redis3("Redis-3");


    RedisCluster cluster;


    // Add Redis servers
    cluster.addServer(&redis1);
    cluster.addServer(&redis2);
    cluster.addServer(&redis3);


    // Store data
    cluster.set("user:1", "Vishal");
    cluster.set("user:2", "Rahul");
    cluster.set("user:3", "Aman");
    cluster.set("user:4", "Rohit");


    // Read data
    cout << "\nResult: "
         << cluster.get("user:1")
         << endl;

    cout << "\nResult: "
         << cluster.get("user:2")
         << endl;

    cout << "\nResult: "
         << cluster.get("user:3")
         << endl;


    return 0;
}