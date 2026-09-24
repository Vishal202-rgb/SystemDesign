#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;


// -------------------------
// Partition
// -------------------------
class Partition {

private:
    int partitionId;
    unordered_map<int, string> data;

public:

    Partition(int id) {
        partitionId = id;
    }

    // Store data
    void insert(int userId, string name) {
        data[userId] = name;
    }

    // Get data
    string get(int userId) {

        if (data.find(userId) != data.end()) {
            return data[userId];
        }

        return "Not Found";
    }

    void showData() {

        cout << "Partition " << partitionId << ": ";

        for (auto &entry : data) {
            cout << "[" << entry.first
                 << " -> " << entry.second << "] ";
        }

        cout << endl;
    }
};


// -------------------------
// Partition Manager
// -------------------------
class Database {

private:
    vector<Partition> partitions;

public:

    Database(int numberOfPartitions) {

        for (int i = 0; i < numberOfPartitions; i++) {
            partitions.emplace_back(i);
        }
    }


    // Decide partition using user ID range
    int getPartition(int userId) {

        if (userId <= 3)
            return 0;

        if (userId <= 6)
            return 1;

        return 2;
    }


    // Insert user
    void insert(int userId, string name) {

        int partitionId = getPartition(userId);

        cout << "User " << userId
             << " -> Partition "
             << partitionId << endl;

        partitions[partitionId].insert(userId, name);
    }


    // Get user
    string get(int userId) {

        int partitionId = getPartition(userId);

        return partitions[partitionId].get(userId);
    }


    // Show all partitions
    void showPartitions() {

        cout << "\nDatabase Partitions:\n";

        for (auto &partition : partitions) {
            partition.showData();
        }
    }
};


int main() {

    // Create database with 3 partitions
    Database database(3);


    // Insert users
    database.insert(1, "Vishal");
    database.insert(2, "Rahul");
    database.insert(3, "Aman");

    database.insert(4, "Rohit");
    database.insert(5, "Karan");
    database.insert(6, "Priya");

    database.insert(7, "Ankit");
    database.insert(8, "Neha");
    database.insert(9, "Pooja");


    // Display partitions
    database.showPartitions();


    // Read users
    cout << "\nUser 2: "
         << database.get(2)
         << endl;

    cout << "User 5: "
         << database.get(5)
         << endl;

    cout << "User 8: "
         << database.get(8)
         << endl;


    return 0;
}