#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;


// --------------------
// Database Server
// --------------------
class Database {

private:
    unordered_map<int, string> data;

public:

    void write(int id, string value) {
        data[id] = value;
    }

    string read(int id) {

        if (data.find(id) != data.end())
            return data[id];

        return "Not Found";
    }

    // Copy data to replica
    unordered_map<int, string> getData() {
        return data;
    }
};


// --------------------
// Read Replica
// --------------------
class ReadReplica {

private:
    unordered_map<int, string> data;

public:

    void sync(unordered_map<int, string> primaryData) {
        data = primaryData;
    }

    string read(int id) {

        if (data.find(id) != data.end())
            return data[id];

        return "Not Found";
    }
};


int main() {

    // Primary database
    Database primary;

    // Two read replicas
    ReadReplica replica1;
    ReadReplica replica2;


    // -------------------------
    // Write goes to Primary
    // -------------------------

    primary.write(1, "Vishal");
    primary.write(2, "Rahul");
    primary.write(3, "Aman");


    // -------------------------
    // Replicate data
    // -------------------------

    replica1.sync(primary.getData());
    replica2.sync(primary.getData());


    // -------------------------
    // Read from replicas
    // -------------------------

    cout << "Replica 1: "
         << replica1.read(1)
         << endl;

    cout << "Replica 2: "
         << replica2.read(2)
         << endl;


    // -------------------------
    // New write
    // -------------------------

    primary.write(4, "Rohit");


    // Replicas need synchronization
    replica1.sync(primary.getData());
    replica2.sync(primary.getData());


    cout << "Replica 1: "
         << replica1.read(4)
         << endl;


    return 0;
}