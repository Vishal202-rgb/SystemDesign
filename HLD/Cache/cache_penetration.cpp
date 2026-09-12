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

    Database() {
        data[1] = "Vishal";
        data[2] = "Rahul";
        data[3] = "Aman";
    }

    string get(int key) {

        cout << "Database: Querying key "
             << key << endl;

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

    bool contains(int key) {
        return data.find(key) != data.end();
    }

    string get(int key) {
        return data[key];
    }

    void put(int key, string value) {
        data[key] = value;
    }
};


// --------------------
// Application
// --------------------
class Application {

private:
    Cache cache;
    Database db;

public:

    string getUser(int key) {

        // 1. Check cache
        if (cache.contains(key)) {

            cout << "Cache Hit" << endl;

            return cache.get(key);
        }

        cout << "Cache Miss" << endl;


        // 2. Query database
        string value = db.get(key);


        // 3. Data does not exist
        if (value == "") {

            cout << "User not found" << endl;

            // Negative caching
            // Store empty value in cache
            cache.put(key, "");

            return "";
        }


        // 4. Store valid data
        cache.put(key, value);

        return value;
    }
};


int main() {

    Application app;


    // Existing user
    cout << "\nRequest 1:\n";
    cout << app.getUser(1) << endl;


    // Non-existing user
    cout << "\nRequest 2:\n";
    cout << app.getUser(999) << endl;


    // Same non-existing user again
    cout << "\nRequest 3:\n";
    cout << app.getUser(999) << endl;


    return 0;
}