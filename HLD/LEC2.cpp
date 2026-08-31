/*Consistent Hashing:Consistent hashing decides which physical server owns a key. Virtual nodes improve distribution. Replication means the key is also stored on the next R-1 distinct physical nodes around the ring.*/

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <set>

using namespace std;


// ======================================================
// SERVER
// ======================================================

class Server {

public:

    string id;

    Server(string id) {
        this->id = id;
    }
};


// ======================================================
// CONSISTENT HASHING
// ======================================================

class ConsistentHash {

private:

    // Hash position -> Server ID
    map<size_t, string> ring;

    // Physical servers
    map<string, Server*> servers;

    // Number of virtual nodes per server
    static const int VIRTUAL_NODES = 16;


    // --------------------------------------------------
    // Hash function
    // --------------------------------------------------

    size_t hashKey(string key) {

        return hash<string>{}(key);
    }


    // --------------------------------------------------
    // Create virtual node name
    // --------------------------------------------------

    string virtualNodeName(string serverId, int index) {

        return serverId + "#VN" + to_string(index);
    }


public:

    // ==================================================
    // ADD SERVER
    // ==================================================

    void addServer(Server* server) {

        servers[server->id] = server;

        for (int i = 0; i < VIRTUAL_NODES; i++) {

            string vnode =
                virtualNodeName(server->id, i);

            size_t hashValue = hashKey(vnode);

            ring[hashValue] = server->id;
        }

        cout << "Added Server: "
             << server->id << endl;
    }


    // ==================================================
    // REMOVE SERVER
    // ==================================================

    void removeServer(string serverId) {

        for (auto it = ring.begin(); it != ring.end();) {

            if (it->second == serverId) {
                it = ring.erase(it);
            }
            else {
                ++it;
            }
        }

        servers.erase(serverId);

        cout << "Removed Server: "
             << serverId << endl;
    }


    // ==================================================
    // FIND PRIMARY SERVER
    // ==================================================

    string getServer(string key) {

        if (ring.empty()) {
            return "No Server Available";
        }

        size_t hashValue = hashKey(key);

        // Find first virtual node clockwise
        auto it = ring.lower_bound(hashValue);

        // If we reach the end,
        // wrap around to beginning
        if (it == ring.end()) {
            it = ring.begin();
        }

        return it->second;
    }


    // ==================================================
    // FIND REPLICAS
    // ==================================================

    vector<string> getReplicas(
        string key,
        int replicationFactor
    ) {

        vector<string> result;

        if (ring.empty()) {
            return result;
        }

        size_t hashValue = hashKey(key);

        auto it = ring.lower_bound(hashValue);

        if (it == ring.end()) {
            it = ring.begin();
        }

        set<string> usedServers;


        // Walk clockwise around the ring
        // until we get enough DISTINCT physical servers

        do {

            string serverId = it->second;

            if (usedServers.find(serverId)
                == usedServers.end()) {

                usedServers.insert(serverId);

                result.push_back(serverId);
            }

            ++it;

            if (it == ring.end()) {
                it = ring.begin();
            }

        } while (
            usedServers.size() < replicationFactor
            && it->first != ring.begin()->first
        );


        return result;
    }
};


// ======================================================
// MAIN
// ======================================================

int main() {

    ConsistentHash hashRing;


    // Physical servers

    Server s1("Server-A");
    Server s2("Server-B");
    Server s3("Server-C");
    Server s4("Server-D");


    // Add servers

    hashRing.addServer(&s1);
    hashRing.addServer(&s2);
    hashRing.addServer(&s3);
    hashRing.addServer(&s4);


    cout << "\n============================\n";
    cout << "KEY DISTRIBUTION\n";
    cout << "============================\n";


    vector<string> keys = {
        "user:101",
        "user:102",
        "user:103",
        "user:104",
        "user:105",
        "product:501",
        "product:502",
        "order:9001"
    };


    // Find primary server

    for (string key : keys) {

        cout << key
             << " -> "
             << hashRing.getServer(key)
             << endl;
    }


    cout << "\n============================\n";
    cout << "REPLICATION\n";
    cout << "============================\n";


    string key = "user:101";

    vector<string> replicas =
        hashRing.getReplicas(key, 3);


    cout << key << " stored on:\n";

    for (string server : replicas) {

        cout << "  " << server << endl;
    }


    cout << "\n============================\n";
    cout << "REMOVE SERVER\n";
    cout << "============================\n";


    hashRing.removeServer("Server-B");


    cout << "\nAfter removing Server-B:\n";

    for (string key : keys) {

        cout << key
             << " -> "
             << hashRing.getServer(key)
             << endl;
    }


    return 0;
}

/*
                 CONSISTENT HASHING
                         │
             ┌───────────┴───────────┐
             ↓                       ↓
       Virtual Nodes             Replication
             ↓                       ↓
        Better Balance          Fault Tolerance
             │                       │
             └───────────┬───────────┘
                         ↓
                  Server Failure
                         ↓
                  Minimal Remapping
                         ↓
                 Weighted Nodes
*/