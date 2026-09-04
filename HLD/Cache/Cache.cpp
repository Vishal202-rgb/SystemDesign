#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;


// ================= CACHE =================

class Cache {

private:

    unordered_map<string, string> data;

public:

    // Store data in cache
    void put(string key, string value) {

        data[key] = value;

        cout << "Data stored in Cache\n";
    }


    // Get data from cache
    string get(string key) {

        if (data.find(key) != data.end()) {

            cout << "CACHE HIT\n";

            return data[key];
        }

        cout << "CACHE MISS\n";

        return "";
    }
};


// ================= DATABASE =================

class Database {

public:

    string getData(string key) {

        cout << "Fetching data from Database...\n";

        if (key == "user:1") {
            return "Vishal";
        }

        if (key == "user:2") {
            return "Rahul";
        }

        return "";
    }
};


// ================= MAIN =================

int main() {

    Cache cache;
    Database database;


    string key = "user:1";


    // Step 1: Check Cache

    string data = cache.get(key);


    // Step 2: Cache Miss

    if (data == "") {

        // Get from Database
        data = database.getData(key);


        // If Database also doesn't have data
        if (data == "") {

            cout << "Data Not Found\n";

            return 0;
        }


        // Store Database result in Cache
        cache.put(key, data);
    }


    // Step 3: Return data

    cout << "Response: " << data << endl;


    // =================================
    // SECOND REQUEST
    // =================================

    cout << "\n----- SECOND REQUEST -----\n";

    data = cache.get(key);

    if (data == "") {

        data = database.getData(key);

        if (data != "") {
            cache.put(key, data);
        }
    }

    cout << "Response: " << data << endl;


    return 0;
}