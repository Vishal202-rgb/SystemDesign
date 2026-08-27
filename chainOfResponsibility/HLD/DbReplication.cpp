#include <iostream>
#include <vector>
using namespace std;

class Database {

public:

    int id;

    Database(int id) {
        this->id = id;
    }

    void write(string data) {
        cout << "Writing '" << data
             << "' to DB " << id << endl;
    }

    void read() {
        cout << "Reading from DB " << id << endl;
    }
};

int main() {

    Database primary(1);
    Database replica1(2);
    Database replica2(3);

    // Write goes to primary
    primary.write("User Vishal");

    // Read can go to replicas
    replica1.read();
    replica2.read();
}