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

        cout << "Database: Saved " << key
             << " -> " << value << endl;
    }

    string get(int key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }
};


// --------------------
// Write-Through Cache
// --------------------
class WriteThroughCache {

private:
    unordered_map<int, string> cache;
    Database* db;

public:

    WriteThroughCache(Database* db) {
        this->db = db;
    }


    void put(int key, string value) {

        // Step 1: Update cache
        cache[key] = value;

        cout << "Cache: Updated " << key
             << " -> " << value << endl;


        // Step 2: Immediately update database
        db->save(key, value);
    }


    string get(int key) {

        // Check cache first
        if (cache.find(key) != cache.end()) {

            cout << "Cache Hit" << endl;

            return cache[key];
        }


        // Cache miss → get from database
        cout << "Cache Miss" << endl;

        string value = db->get(key);

        // Store database result in cache
        if (value != "") {
            cache[key] = value;
        }

        return value;
    }
};


int main() {

    Database db;

    WriteThroughCache cache(&db);


    // Write
    cache.put(1, "Vishal");


    // Read
    cout << cache.get(1) << endl;


    return 0;
}