#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class Cache {

private:

    unordered_map<string, string> data;

public:

    void put(string key, string value) {

        data[key] = value;

        cout << "Data stored in cache: "
             << key << endl;
    }


    string get(string key) {

        if (data.find(key) != data.end()) {

            cout << "CACHE HIT: "
                 << key << endl;

            return data[key];
        }

        cout << "CACHE MISS: "
             << key << endl;

        return "";
    }
};


class Database {

public:

    string getData(string key) {

        cout << "Fetching from Database...\n";

        if (key == "user:1") {
            return "Vishal";
        }

        if (key == "user:2") {
            return "Rahul";
        }

        return "User Not Found";
    }
};


int main() {

    Cache cache;
    Database database;


    // First request
    cout << "\n----- REQUEST 1 -----\n";

    string data = cache.get("user:1");

    if (data == "") {

        data = database.getData("user:1");

        cache.put("user:1", data);
    }

    cout << "Response: " << data << endl;


    // Second request
    cout << "\n----- REQUEST 2 -----\n";

    data = cache.get("user:1");

    if (data == "") {

        data = database.getData("user:1");

        cache.put("user:1", data);
    }

    cout << "Response: " << data << endl;


    return 0;
}
/*
                    USERS
                      |
                      ↓
                LOAD BALANCER
                      |
             ┌────────┼────────┐
             ↓        ↓        ↓
           S1        S2       S3
             \        |        /
              \       |       /
               └──────┼──────┘
                      ↓
                    CACHE
                      |
                  CACHE HIT?
                   /      \
                 YES       NO
                  |         |
                  ↓         ↓
               Response   DATABASE
                            |
                            ↓
                          CACHE
*/