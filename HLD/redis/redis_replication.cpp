#include <iostream>
#include <unordered_map>

using namespace std;


// --------------------
// Redis Server
// --------------------
class RedisServer {

private:

    string name;

    unordered_map<string, string> data;

public:

    RedisServer(string name) {
        this->name = name;
    }


    void set(string key, string value) {

        data[key] = value;

        cout << name
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


    string getName() {
        return name;
    }
};


// --------------------
// Redis Replication
// --------------------
class RedisReplication {

private:

    RedisServer* primary;

    RedisServer* replica1;
    RedisServer* replica2;


public:

    RedisReplication(
        RedisServer* primary,
        RedisServer* replica1,
        RedisServer* replica2
    ) {
        this->primary = primary;
        this->replica1 = replica1;
        this->replica2 = replica2;
    }


    // Write to primary
    // and replicate to replicas
    void set(string key, string value) {

        cout << "\nWriting data...\n";

        // Write to primary
        primary->set(key, value);

        // Copy data to replicas
        replica1->set(key, value);
        replica2->set(key, value);
    }


    // Read from primary
    string get(string key) {

        return primary->get(key);
    }


    // Read from replica
    string getFromReplica(string key) {

        string value = replica1->get(key);

        if (value != "") {
            return value;
        }

        return replica2->get(key);
    }
};


// --------------------
// Main
// --------------------
int main() {

    RedisServer primary("Redis-Primary");

    RedisServer replica1("Redis-Replica-1");

    RedisServer replica2("Redis-Replica-2");


    RedisReplication redis(
        &primary,
        &replica1,
        &replica2
    );


    // Write data
    redis.set("user:1", "Vishal");


    // Read from primary
    cout << "\nRead from Primary: "
         << redis.get("user:1")
         << endl;


    // Read from replica
    cout << "Read from Replica: "
         << redis.getFromReplica("user:1")
         << endl;


    return 0;
}