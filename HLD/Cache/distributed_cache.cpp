#include <iostream>
#include <unordered_map>

using namespace std;


// --------------------
// Distributed Cache
// --------------------
class DistributedCache {

private:

    // Shared cache
    unordered_map<int, string> data;


public:

    void put(int key, string value) {

        data[key] = value;

        cout << "Cache: Stored "
             << key << " -> "
             << value << endl;
    }


    string get(int key) {

        if (data.find(key) == data.end()) {

            cout << "Cache Miss" << endl;

            return "";
        }

        cout << "Cache Hit" << endl;

        return data[key];
    }
};


// --------------------
// Application Server
// --------------------
class Server {

private:

    int id;
    DistributedCache* cache;


public:

    Server(int id, DistributedCache* cache) {

        this->id = id;
        this->cache = cache;
    }


    void saveData(int key, string value) {

        cout << "\nServer " << id
             << " writing data\n";

        cache->put(key, value);
    }


    void getData(int key) {

        cout << "\nServer " << id
             << " requesting data\n";

        string value = cache->get(key);

        if (value != "") {

            cout << "Server " << id
                 << " received: "
                 << value << endl;
        }
    }
};


int main() {

    // One shared distributed cache
    DistributedCache cache;


    // Multiple servers
    Server server1(1, &cache);
    Server server2(2, &cache);
    Server server3(3, &cache);


    // Server 1 stores data
    server1.saveData(101, "Vishal");


    // Server 2 can access the same data
    server2.getData(101);


    // Server 3 can also access it
    server3.getData(101);


    return 0;
}

//Distributed Cache = multiple servers + one shared cache.