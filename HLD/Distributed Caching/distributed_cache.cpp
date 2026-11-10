#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

// Shared Distributed Cache
class DistributedCache {
private:
    unordered_map<string, string> cache;

public:
    // Get data from cache
    bool get(string key, string& value) {
        if (cache.find(key) != cache.end()) {
            value = cache[key];
            return true;
        }

        return false;
    }

    // Store data in cache
    void set(string key, string value) {
        cache[key] = value;
        cout << "Cache Updated: " << key << "\n";
    }

    // Delete data from cache
    void remove(string key) {
        cache.erase(key);
        cout << "Cache Removed: " << key << "\n";
    }
};

// Simulated Database
class Database {
private:
    unordered_map<string, string> data;

public:
    Database() {
        data["user1"] = "Vishal";
        data["user2"] = "Rahul";
    }

    string getData(string key) {
        cout << "Fetching from Database: " << key << "\n";

        if (data.find(key) != data.end()) {
            return data[key];
        }

        return "NOT FOUND";
    }
};

// Application Server
class ApplicationServer {
private:
    string serverName;
    DistributedCache& cache;
    Database& database;

public:
    ApplicationServer(
        string name,
        DistributedCache& c,
        Database& db
    ) : serverName(name), cache(c), database(db) {}

    void getUser(string userId) {
        cout << "\n" << serverName
             << " requesting: " << userId << "\n";

        string value;

        // First check shared cache
        if (cache.get(userId, value)) {
            cout << "Cache HIT: " << value << "\n";
        } else {
            cout << "Cache MISS\n";

            // Fetch from database
            value = database.getData(userId);

            if (value != "NOT FOUND") {
                // Update shared cache
                cache.set(userId, value);
            }
        }

        cout << "Final Result: " << value << "\n";
    }
};

int main() {
    // One shared cache and database
    DistributedCache sharedCache;
    Database database;

    // Multiple application servers
    ApplicationServer server1(
        "Server 1", sharedCache, database
    );

    ApplicationServer server2(
        "Server 2", sharedCache, database
    );

    // Server 1 requests user1
    server1.getUser("user1");

    // Server 2 requests the same user
    server2.getUser("user1");

    // Server 2 requests another user
    server2.getUser("user2");

    // Server 1 requests the same user again
    server1.getUser("user2");

    return 0;
}
