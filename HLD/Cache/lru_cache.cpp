#include <iostream>
#include <unordered_map>
using namespace std;

class LRUCache {

private:

    struct Node {

        int key;
        int value;

        Node* prev;
        Node* next;

        Node(int key, int value) {

            this->key = key;
            this->value = value;

            prev = nullptr;
            next = nullptr;
        }
    };


    int capacity;

    unordered_map<int, Node*> cache;

    Node* head;
    Node* tail;


    // Remove a node
    void removeNode(Node* node) {

        node->prev->next = node->next;
        node->next->prev = node->prev;
    }


    // Add node just before tail
    void addToFront(Node* node) {

        node->next = head->next;
        node->prev = head;

        head->next->prev = node;
        head->next = node;
    }


public:

    LRUCache(int capacity) {

        this->capacity = capacity;

        // Dummy nodes
        head = new Node(-1, -1);
        tail = new Node(-1, -1);

        head->next = tail;
        tail->prev = head;
    }


    int get(int key) {

        // Key not present
        if (cache.find(key) == cache.end()) {

            cout << "CACHE MISS: " << key << endl;

            return -1;
        }


        Node* node = cache[key];

        // This node was just used,
        // so remove it from its old position
        removeNode(node);

        // Put it at most recently used position
        addToFront(node);

        cout << "CACHE HIT: " << key << endl;

        return node->value;
    }


    void put(int key, int value) {

        // Key already exists
        if (cache.find(key) != cache.end()) {

            Node* node = cache[key];

            node->value = value;

            removeNode(node);
            addToFront(node);

            return;
        }


        // Create new node
        Node* node = new Node(key, value);

        cache[key] = node;

        addToFront(node);


        // Capacity exceeded
        if (cache.size() > capacity) {

            Node* lru = tail->prev;

            cache.erase(lru->key);

            removeNode(lru);

            delete lru;
        }
    }


    void display() {

        Node* current = head->next;

        cout << "Cache: ";

        while (current != tail) {

            cout << current->key << " ";

            current = current->next;
        }

        cout << endl;
    }
};


int main() {

    LRUCache cache(3);


    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    cache.display();


    // Use key 1
    cache.get(1);

    cache.display();


    // Add key 4
    cache.put(4, 400);

    cache.display();


    // Try key 2
    cache.get(2);

    return 0;
}