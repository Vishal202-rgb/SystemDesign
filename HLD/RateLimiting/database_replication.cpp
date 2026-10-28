#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Database {
private:
    unordered_map<string, string> data;

public:
    Database() = default;

    void write(string key, string value) {
        data[key] = value;
        cout << "Written: " << key << " = " << value << "\n";
    }

    string read(string key) {
        if (data.find(key) != data.end()) {
            return data[key];
        }
        return "NOT FOUND";
    }

    void copyFrom(const Database& primary) {
        data = primary.data;
        cout << "Replica synchronized with primary.\n";
    }
};

int main() {
    Database primaryDB;
    Database replicaDB;

    // Write data to the primary database
    primaryDB.write("user1", "Vishal");

    // Replicate data to the replica
    replicaDB.copyFrom(primaryDB);

    // Read from the replica
    cout << "Replica read: "
         << replicaDB.read("user1") << "\n";

    // Update primary database
    primaryDB.write("user1", "Rahul");

    // Before synchronization, replica has stale data
    cout << "Before sync: "
         << replicaDB.read("user1") << "\n";

    // Synchronize again
    replicaDB.copyFrom(primaryDB);

    cout << "After sync: "
         << replicaDB.read("user1") << "\n";

    return 0;
}
