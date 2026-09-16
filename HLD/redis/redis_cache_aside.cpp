#include <iostream>
#include <unordered_map>

using namespace std;


// --------------------
// Database
// --------------------
class Database {

private:

    unordered_map<string, string> data;

public:

    Database() {
        data["user:1"] = "Vishal";
        data["user:2"] = "Rahul";
    }


    string get(string key) {

        cout << "Database: Fetching " << key << endl;

        if (data.find(key) == data.end()) {
            return "";
        }

        return data[key];
    }


    void save(string key, string value) {

        data[key] = value;

        cout << "Database: Saved "
             << key << " -> "
             << value << endl;
    }
};


// --------------------
// Redis
// --------------------
class Redis {

private:

    unordered_map<string, string> data;

public:

    string get(string key) {

        if (data.find(key) == data.end()) {

            cout << "Redis: Cache Miss" << endl;

            return "";
        }

        cout << "Redis: Cache Hit" << endl;

        return data[key];
    }


    void set(string key, string value) {

        data[key] = value;

        cout << "Redis: Stored "
             << key << " -> "
             << value << endl;
    }


    void remove(string key) {

        data.erase(key);
    }
};


// --------------------
// Application
// --------------------
class Application {

private:

    Redis redis;
    Database db;

public:

    string getUser(string key) {

        // 1. Check Redis
        string value = redis.get(key);

        if (value != "") {

            return value;
        }


        // 2. Cache Miss → Database
        value = db.get(key);

        if (value != "") {

            // 3. Store DB result in Redis
            redis.set(key, value);
        }


        // 4. Return result
        return value;
    }


    void updateUser(string key, string value) {

        // Update database
        db.save(key, value);

        // Invalidate Redis
        redis.remove(key);

        cout << "Redis: Cache invalidated" << endl;
    }
};


// --------------------
// Main
// --------------------
int main() {

    Application app;


    cout << "\n--- First Request ---\n";

    cout << "Result: "
         << app.getUser("user:1")
         << endl;


    cout << "\n--- Second Request ---\n";

    cout << "Result: "
         << app.getUser("user:1")
         << endl;


    cout << "\n--- Updating User ---\n";

    app.updateUser("user:1", "Amit");


    cout << "\n--- Third Request ---\n";

    cout << "Result: "
         << app.getUser("user:1")
         << endl;


    return 0;
}