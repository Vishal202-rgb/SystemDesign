#include <iostream>
#include <unordered_map>

using namespace std;


// --------------------
// Database
// --------------------
class Database {

private:
    unordered_map<int, string> data;

public:

    void save(int key, string value) {

        data[key] = value;

        cout << "Database: Saved "
             << key << " -> " << value << endl;
    }

    string get(int key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }
};


// --------------------
// Write-Back Cache
// --------------------
class WriteBackCache {

private:

    unordered_map<int, string> cache;

    // Tracks data that has not yet
    // been written to database
    unordered_map<int, bool> dirty;

    Database* db;


public:

    WriteBackCache(Database* db) {

        this->db = db;
    }


    // Write to cache only
    void put(int key, string value) {

        cache[key] = value;

        // Mark as dirty
        dirty[key] = true;

        cout << "Cache: Updated "
             << key << " -> " << value << endl;
    }


    // Read from cache
    string get(int key) {

        if (cache.find(key) != cache.end()) {

            cout << "Cache Hit" << endl;

            return cache[key];
        }


        // Cache miss
        cout << "Cache Miss" << endl;

        string value = db->get(key);

        if (value != "") {
            cache[key] = value;
        }

        return value;
    }


    // Write dirty data to database
    void flush() {

        cout << "\nFlushing cache to database...\n";

        for (auto &entry : dirty) {

            int key = entry.first;

            if (entry.second) {

                db->save(key, cache[key]);

                // Data is now synchronized
                entry.second = false;
            }
        }
    }
};


int main() {

    Database db;

    WriteBackCache cache(&db);


    // These writes happen only in cache
    cache.put(1, "Vishal");
    cache.put(2, "Rahul");


    cout << "\nReading from cache:\n";

    cout << cache.get(1) << endl;


    // Later, synchronize cache with database
    cache.flush();


    return 0;
}