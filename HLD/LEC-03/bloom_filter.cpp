#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class BloomFilter {

private:
    vector<bool> bits;
    int size;
    int hashCount;

    // Generate different hash values
    int getHash(const string& key, int seed) {
        string value = key + to_string(seed);

        return hash<string>{}(value) % size;
    }

public:

    // Constructor
    BloomFilter(int size, int hashCount) {
        this->size = size;
        this->hashCount = hashCount;

        bits.resize(size, false);
    }

    // Add an item
    void add(const string& key) {

        for (int i = 0; i < hashCount; i++) {

            int index = getHash(key, i);

            bits[index] = true;
        }
    }

    // Check whether an item may exist
    bool mightContain(const string& key) {

        for (int i = 0; i < hashCount; i++) {

            int index = getHash(key, i);

            // One bit is false -> definitely not present
            if (!bits[index]) {
                return false;
            }
        }

        // All bits are true -> may be present
        return true;
    }
};


int main() {

    // Create Bloom Filter
    // 20 bits and 3 hash functions
    BloomFilter filter(20, 3);


    // Add some items
    filter.add("vishal");
    filter.add("rahul");
    filter.add("aman");


    // Check items
    cout << "vishal: "
         << (filter.mightContain("vishal")
             ? "MAYBE PRESENT"
             : "NOT PRESENT")
         << endl;


    cout << "rahul: "
         << (filter.mightContain("rahul")
             ? "MAYBE PRESENT"
             : "NOT PRESENT")
         << endl;


    cout << "aman: "
         << (filter.mightContain("aman")
             ? "MAYBE PRESENT"
             : "NOT PRESENT")
         << endl;


    // This item was never added.
    // It may still return MAYBE because
    // of a false positive.
    cout << "rohit: "
         << (filter.mightContain("rohit")
             ? "MAYBE PRESENT"
             : "NOT PRESENT")
         << endl;


    return 0;
}