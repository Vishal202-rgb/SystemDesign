#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

using namespace std;


// --------------------
// Redis Shard
// --------------------
class RedisShard {

private:
    int id;
    unordered_map<string, string> data;

public:

    RedisShard(int id) {
        this->id = id;
    }

    void set(string key, string value) {
        data[key] = value;

        cout << "Redis-" << id
             << " SET: "
             << key << " -> "
             << value << endl;
    }

    string get(string key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }

    int getId() {
        return id;
    }
};


// --------------------
// Redis Sharding
// --------------------
class RedisCluster {

private:

    vector<RedisShard*> shards;


    // Decide which shard stores the key
    int getShard(string key) {

        size_t hashValue = hash<string>{}(key);

        return hashValue % shards.size();
    }


public:

    void addShard(RedisShard* shard) {
        shards.push_back(shard);
    }


    void set(string key, string value) {

        if (shards.empty()) {
            cout << "No Redis shard available\n";
            return;
        }

        int index = getShard(key);

        cout << "\nKey " << key
             << " routed to Redis-"
             << shards[index]->getId() << endl;

        shards[index]->set(key, value);
    }


    string get(string key) {

        if (shards.empty()) {
            return "";
        }

        int index = getShard(key);

        cout << "\nKey " << key
             << " routed to Redis-"
             << shards[index]->getId() << endl;

        string value = shards[index]->get(key);

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

    RedisShard redis1(1);
    RedisShard redis2(2);
    RedisShard redis3(3);


    RedisCluster cluster;

    cluster.addShard(&redis1);
    cluster.addShard(&redis2);
    cluster.addShard(&redis3);


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

    cout << "\nResult: "
         << cluster.get("user:4")
         << endl;

    return 0;
}