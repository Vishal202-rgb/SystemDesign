#include <iostream>
#include <unordered_map>
#include <string>
#include <chrono>

using namespace std;

class Cache {

private:

    struct CacheEntry {
        string value;
        chrono::steady_clock::time_point expiry;
    };

    unordered_map<string, CacheEntry> data;

public:

    // Store data with TTL in seconds
    void put(string key, string value, int ttlSeconds) {

        auto expiryTime =
            chrono::steady_clock::now()
            + chrono::seconds(ttlSeconds);

        data[key] = {value, expiryTime};

        cout << "Stored in cache: "
             << key << endl;
    }


    string get(string key) {

        // Key doesn't exist
        if (data.find(key) == data.end()) {

            cout << "CACHE MISS\n";

            return "";
        }


        auto now = chrono::steady_clock::now();

        CacheEntry entry = data[key];


        // Check expiration
        if (now >= entry.expiry) {

            cout << "CACHE EXPIRED\n";

            data.erase(key);

            return "";
        }


        // Cache hit
        cout << "CACHE HIT\n";

        return entry.value;
    }
};


int main() {

    Cache cache;


    // Store user for 5 seconds

    cache.put(
        "user:1",
        "Vishal",
        5
    );


    // Immediately request

    cout << "\nFirst request:\n";

    cout << cache.get("user:1") << endl;


    // Wait 6 seconds

    cout << "\nWaiting...\n";

    // In real testing:
    // this is where 6 seconds would pass


    // Example:
    // this_thread::sleep_for(chrono::seconds(6));


    return 0;
}