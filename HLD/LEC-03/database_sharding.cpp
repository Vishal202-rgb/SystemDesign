#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;


// -------------------------
// Database Shard
// -------------------------
class DatabaseShard {

private:
    int shardId;
    unordered_map<int, string> data;

public:

    DatabaseShard(int id) {
        shardId = id;
    }

    // Store data in this shard
    void insert(int userId, string name) {
        data[userId] = name;
    }

    // Read data from this shard
    string get(int userId) {

        if (data.find(userId) != data.end()) {
            return data[userId];
        }

        return "Not Found";
    }

    void showData() {

        cout << "Shard " << shardId << ": ";

        for (auto &entry : data) {
            cout << "[" << entry.first
                 << " -> " << entry.second << "] ";
        }

        cout << endl;
    }
};


// -------------------------
// Shard Manager
// -------------------------
class ShardManager {

private:
    vector<DatabaseShard> shards;
    int numberOfShards;

    // Decide which shard stores the user
    int getShard(int userId) {
        return userId % numberOfShards;
    }

public:

    ShardManager(int count) {

        numberOfShards = count;

        for (int i = 0; i < count; i++) {
            shards.emplace_back(i);
        }
    }


    // Insert user
    void insert(int userId, string name) {

        int shardId = getShard(userId);

        cout << "User " << userId
             << " -> Shard " << shardId << endl;

        shards[shardId].insert(userId, name);
    }


    // Find user
    string get(int userId) {

        int shardId = getShard(userId);

        return shards[shardId].get(userId);
    }


    // Display all shards
    void showAllShards() {

        cout << "\nDatabase Shards:\n";

        for (auto &shard : shards) {
            shard.showData();
        }
    }
};


int main() {

    // Create 3 database shards
    ShardManager database(3);


    // Insert users
    database.insert(1, "Vishal");
    database.insert(2, "Rahul");
    database.insert(3, "Aman");
    database.insert(4, "Rohit");
    database.insert(5, "Karan");
    database.insert(6, "Priya");


    // Show data distribution
    database.showAllShards();


    // Read user
    cout << "\nUser 4: "
         << database.get(4)
         << endl;

    cout << "User 5: "
         << database.get(5)
         << endl;


    return 0;
}