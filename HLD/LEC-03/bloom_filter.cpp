#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class BloomFilter {

private:

    vector<bool> bits;
    int size;

    // First hash function
    int hash1(const string& key) {
        return hash<string>{}(key) % size;
    }

    // Second hash function
    int hash2(const string& key) {
        return (hash<string>{}(key + "salt")) % size;
    }

public:

    BloomFilter(int size) {
        this->size = size;
        bits.resize(size, false);
    }

    // Add an item
    void add(const string& key) {

        int index1 = hash1(key);
        int index2 = hash2(key);

        bits[index1] = true;
        bits[index2] = true;
    }

    // Check whether item may exist
    bool mightContain(const string& key) {

        int index1 = hash1(key);
        int index2 = hash2(key);

        return bits[index1] && bits[index2];
    }
};


int main() {

    BloomFilter filter(10);

    // Add users
    filter.add("vishal");
    filter.add("rahul");
    filter.add("aman");


    // Check users
    cout << "vishal: "
         << filter.mightContain("vishal")
         << endl;

    cout << "rahul: "
         << filter.mightContain("rahul")
         << endl;

    cout << "aman: "
         << filter.mightContain("aman")
         << endl;


    return 0;
}