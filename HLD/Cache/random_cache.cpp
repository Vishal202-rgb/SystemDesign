#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class RandomCache {

private:

    int capacity;

    // key -> value
    unordered_map<int, int> cache;

    // Stores all keys
    vector<int> keys;


public:

    RandomCache(int capacity) {
        this->capacity = capacity;

        // Initialize random seed
        srand(time(0));
    }


    int get(int key) {

        if (cache.find(key) == cache.end()) {
            return -1;
        }

        return cache[key];
    }


    void put(int key, int value) {

        // Cache has no capacity
        if (capacity == 0) {
            return;
        }


        // Key already exists
        if (cache.find(key) != cache.end()) {

            cache[key] = value;

            return;
        }


        // Cache is full
        if (cache.size() == capacity) {

            // Choose random index
            int randomIndex = rand() % keys.size();

            // Get random key
            int keyToRemove = keys[randomIndex];

            // Remove from map
            cache.erase(keyToRemove);

            // Remove from vector
            keys.erase(keys.begin() + randomIndex);
        }


        // Insert new key
        cache[key] = value;
        keys.push_back(key);
    }
};


int main() {

    RandomCache cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    cout << cache.get(1) << endl;

    // Cache is full
    // One of 1, 2, 3 will be randomly removed

    cache.put(4, 400);

    cout << cache.get(4) << endl;

    return 0;
}