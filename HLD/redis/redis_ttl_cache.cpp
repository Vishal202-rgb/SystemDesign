#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>

using namespace std;

class RedisCache {

private:

    struct Entry {
        string value;
        chrono::steady_clock::time_point expiry;
    };

    unordered_map<string, Entry> data;


public:

    // SET key value with TTL
    void set(string key, string value, int ttlSeconds) {

        Entry entry;

        entry.value = value;

        entry.expiry =
            chrono::steady_clock::now()
            + chrono::seconds(ttlSeconds);

        data[key] = entry;

        cout << "Redis SET: "
             << key << " -> "
             << value
             << " | TTL = "
             << ttlSeconds << " sec"
             << endl;
    }


    // GET key
    string get(string key) {

        // Key doesn't exist
        if (data.find(key) == data.end()) {

            cout << "Redis: Cache Miss" << endl;

            return "";
        }


        // Check expiration
        if (chrono::steady_clock::now()
            >= data[key].expiry) {

            cout << "Redis: Key expired" << endl;

            data.erase(key);

            return "";
        }


        cout << "Redis: Cache Hit" << endl;

        return data[key].value;
    }


    // DELETE key
    void remove(string key) {

        data.erase(key);

        cout << "Redis DELETE: "
             << key << endl;
    }
};


int main() {

    RedisCache redis;


    // Store data for 5 seconds
    redis.set(
        "user:1",
        "Vishal",
        5
    );


    // Immediately read
    cout << "\nFirst GET:\n";

    cout << redis.get("user:1")
         << endl;


    // Wait for 6 seconds
    cout << "\nWaiting for expiration...\n";

    this_thread::sleep_for(
        chrono::seconds(6)
    );


    // Read after TTL
    cout << "\nSecond GET:\n";

    cout << redis.get("user:1")
         << endl;


    return 0;
}