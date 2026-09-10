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

        cout << "Database updated: "
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
// Cache
// --------------------
class Cache {

private:
    unordered_map<int, string> data;

public:

    void put(int key, string value) {
        data[key] = value;
    }

    string get(int key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }

    void remove(int key) {
        data.erase(key);

        cout << "Cache invalidated for key: "
             << key << endl;
    }
};


// --------------------
// Application
// --------------------
class Application {

private:
    Database* db;
    Cache* cache;

public:

    Application(Database* db, Cache* cache) {
        this->db = db;
        this->cache = cache;
    }


    // Read data
    string getUser(int key) {

        // 1. Check cache
        string value = cache->get(key);

        if (value != "") {

            cout << "Cache Hit" << endl;

            return value;
        }

        // 2. Cache miss → Database
        cout << "Cache Miss" << endl;

        value = db->get(key);

        // 3. Store latest data in cache
        if (value != "") {
            cache->put(key, value);
        }

        return value;
    }


    // Update data
    void updateUser(int key, string value) {

        // 1. Update database
        db->save(key, value);

        // 2. Invalidate old cache
        cache->remove(key);
    }
};


int main() {

    Database db;
    Cache cache;

    Application app(&db, &cache);


    // Initial database value
    db.save(1, "Vishal");


    // First read
    cout << "\nFirst read:\n";
    cout << app.getUser(1) << endl;


    // Update user
    cout << "\nUpdating user:\n";
    app.updateUser(1, "Rahul");


    // Read again
    cout << "\nSecond read:\n";
    cout << app.getUser(1) << endl;


    return 0;
}