#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LFUCache {

private:

    struct Node {
        int key;
        int value;
        int freq;

        Node(int key, int value) {
            this->key = key;
            this->value = value;
            this->freq = 1;
        }
    };

    int capacity;

    // key -> Node
    unordered_map<int, Node*> keyTable;

    // frequency -> list of nodes
    unordered_map<int, list<Node*>> freqTable;

    // minimum frequency currently present
    int minFreq;


    void increaseFrequency(Node* node) {

        int oldFreq = node->freq;

        // Remove from old frequency list
        freqTable[oldFreq].remove(node);

        // If this was the last node of minFreq
        if (oldFreq == minFreq &&
            freqTable[oldFreq].empty()) {

            minFreq++;
        }

        // Increase frequency
        node->freq++;

        // Add to new frequency list
        freqTable[node->freq].push_front(node);
    }


public:

    LFUCache(int capacity) {

        this->capacity = capacity;
        minFreq = 0;
    }


    int get(int key) {

        // Key does not exist
        if (keyTable.find(key) == keyTable.end()) {
            return -1;
        }

        Node* node = keyTable[key];

        // This key was used again
        increaseFrequency(node);

        return node->value;
    }


    void put(int key, int value) {

        // Cache has no capacity
        if (capacity == 0) {
            return;
        }


        // Key already exists
        if (keyTable.find(key) != keyTable.end()) {

            Node* node = keyTable[key];

            node->value = value;

            increaseFrequency(node);

            return;
        }


        // Cache is full
        if (keyTable.size() == capacity) {

            // Least frequently used list
            auto &leastFreqList = freqTable[minFreq];

            // Last node = least recently used
            Node* nodeToRemove = leastFreqList.back();

            leastFreqList.pop_back();

            // Remove from key table
            keyTable.erase(nodeToRemove->key);

            delete nodeToRemove;
        }


        // Create new node
        Node* newNode = new Node(key, value);

        // New node always starts with frequency 1
        keyTable[key] = newNode;

        freqTable[1].push_front(newNode);

        minFreq = 1;
    }
};


int main() {

    LFUCache cache(3);

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);

    cout << cache.get(1) << endl;
    cout << cache.get(1) << endl;

    cout << cache.get(2) << endl;

    // Frequencies:
    //
    // key 1 -> 3
    // key 2 -> 2
    // key 3 -> 1
    //
    // Cache full.
    // Key 3 should be removed.

    cache.put(4, 40);

    cout << cache.get(3) << endl; // -1
    cout << cache.get(4) << endl; // 40

    return 0;
}