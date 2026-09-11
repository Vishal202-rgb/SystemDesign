#include <iostream>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;


// --------------------
// Database
// --------------------
class Database {

public:

    string get(int key) {

        cout << "Database: Fetching key "
             << key << endl;

        // Simulate database work
        this_thread::sleep_for(
            chrono::milliseconds(100)
        );

        return "Data_" + to_string(key);
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

    // Lock to prevent multiple
    // requests from hitting DB
    mutex mtx;


public:

    string getData(int key) {

        // First check cache
        if (cache.contains(key)) {

            cout << "Cache Hit" << endl;

            return cache.get(key);
        }

        cout << "Cache Miss" << endl;


        // Only one thread can enter here
        lock_guard<mutex> lock(mtx);


        // IMPORTANT:
        // Check cache AGAIN after acquiring lock
        if (cache.contains(key)) {

            cout << "Another request already "
                 << "loaded the data" << endl;

            return cache.get(key);
        }


        // Only this request goes to DB
        string value = db.get(key);


        // Store result in cache
        cache.put(key, value);

        return value;
    }
};


void request(Application &app, int id) {

    cout << "Request received\n";

    string result = app.getData(id);

    cout << "Result: " << result << "\n\n";
}


int main() {

    Application app;


    // Multiple requests asking
    // for the same data
    thread t1(request, ref(app), 1);
    thread t2(request, ref(app), 1);
    thread t3(request, ref(app), 1);
    thread t4(request, ref(app), 1);


    t1.join();
    t2.join();
    t3.join();
    t4.join();


    return 0;
}