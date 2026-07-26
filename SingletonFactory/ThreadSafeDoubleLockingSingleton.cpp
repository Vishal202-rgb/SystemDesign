#include <iostream>

using namespace std;

class Singleton {
private:
    Singleton() {
        cout << "Singleton Constructor Called!" << endl;
    }

    // Prevent copying
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton& getInstance() {
        static Singleton instance;   // Thread-safe in C++11+
        return instance;
    }
};

int main() {
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();

    cout << (&s1 == &s2) << endl;
}