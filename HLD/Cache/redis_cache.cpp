#include <iostream>
#include <unordered_map>
#include <chrono>

using namespace std;


class RedisCache {

private:

    unordered_map<string, string> data;

public:

    // SET key value
    void set(string key, string value) {

        data[key] = value;

        cout << "Redis SET: "
             << key << " -> "
             << value << endl;
    }


    // GET key
    string get(string key) {

        if (data.find(key) == data.end()) {

            cout << "Redis: Cache Miss" << endl;

            return "";
        }

        cout << "Redis: Cache Hit" << endl;

        return data[key];
    }


    // DELETE key
    void remove(string key) {

        data.erase(key);

        cout << "Redis DELETE: "
             << key << endl;
    }
};


class Database {

private:

    unordered_map<string, string> data;

public:

    void save(string key, string value) {

        data[key] = value;

        cout << "Database: Saved "
             << key << " -> "
             << value << endl;
    }


    string get(string key) {

        if (data.find(key) == data.end()) {
            return "";
        }

        cout << "Database: Fetching "
             << key << endl;

        return data[key];
    }
};


class Application {

private:

    RedisCache redis;
    Database db;

public:

    // Read operation
    string getData(string key) {

        // 1. Check Redis
        string value = redis.get(key);

        if (value != "") {

            return value;
        }


        // 2. Redis miss → Database
        value = db.get(key);

        if (value != "") {

            // 3. Store DB result in Redis
            redis.set(key, value);
        }

        return value;
    }


    // Write operation
    void saveData(string key, string value) {

        // Update database
        db.save(key, value);

        // Invalidate old Redis value
        redis.remove(key);
    }
};


int main() {

    Application app;


    // Store data
    app.saveData("user:1", "Vishal");


    cout << "\nFirst request:\n";

    cout << app.getData("user:1")
         << endl;


    cout << "\nSecond request:\n";

    cout << app.getData("user:1")
         << endl;


    return 0;
}