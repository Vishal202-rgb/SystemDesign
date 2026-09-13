#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

class Cache {

private:

    struct Entry {
        string value;
        chrono::steady_clock::time_point expiry;
    };

    unordered_map<int, Entry> data;


public:

    void put(int key, string value, int ttlSeconds) {

        Entry entry;

        entry.value = value;

        entry.expiry =
            chrono::steady_clock::now()
            + chrono::seconds(ttlSeconds);

        data[key] = entry;

        cout << "Cached key " << key
             << " with TTL "
             << ttlSeconds << " seconds\n";
    }


    string get(int key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        // Check expiration
        if (chrono::steady_clock::now()
            >= data[key].expiry) {

            cout << "Key " << key
                 << " expired\n";

            data.erase(key);

            return "";
        }

        return data[key].value;
    }
};


int main() {

    Cache cache;

    srand(time(0));


    // Base TTL
    int baseTTL = 5;


    // Store multiple keys
    for (int i = 1; i <= 5; i++) {

        // Add random time to TTL
        int randomTTL =
            baseTTL + (rand() % 5);

        cache.put(
            i,
            "Data_" + to_string(i),
            randomTTL
        );
    }


    cout << "\nWaiting...\n";

    this_thread::sleep_for(
        chrono::seconds(6)
    );


    cout << "\nChecking cache:\n";


    for (int i = 1; i <= 5; i++) {

        string value = cache.get(i);

        if (value == "") {
            cout << "Key " << i
                 << " -> Cache Miss\n";
        }
        else {
            cout << "Key " << i
                 << " -> Cache Hit: "
                 << value << endl;
        }
    }


    return 0;
}