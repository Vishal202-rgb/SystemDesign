#include <iostream>
#include <unordered_map>
#include <queue>

using namespace std;

class FIFOCache {

private:

    int capacity;

    // key -> value
    unordered_map<int, int> cache;

    // stores keys in insertion order
    queue<int> order;


public:

    FIFOCache(int capacity) {
        this->capacity = capacity;
    }


    int get(int key) {

        // Key not present
        if (cache.find(key) == cache.end()) {
            return -1;
        }

        return cache[key];
    }


    void put(int key, int value) {

        // Key already exists
        if (cache.find(key) != cache.end()) {

            cache[key] = value;

            return;
        }


        // Cache is full
        if (cache.size() == capacity) {

            int oldestKey = order.front();

            order.pop();

            cache.erase(oldestKey);
        }


        // Insert new key
        cache[key] = value;

        order.push(key);
    }
};


int main() {

    FIFOCache cache(3);

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);

    cout << cache.get(1) << endl; // 10

    // Cache is full
    // 1 is the oldest key

    cache.put(4, 40);

    cout << cache.get(1) << endl; // -1
    cout << cache.get(2) << endl; // 20
    cout << cache.get(4) << endl; // 40

    return 0;
}