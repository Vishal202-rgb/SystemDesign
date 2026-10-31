#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class DistributedLock {
private:
    unordered_map<string, string> locks;

public:
    // Acquire lock
    bool acquireLock(string resource, string server) {

        if (locks.find(resource) == locks.end()) {
            locks[resource] = server;

            cout << server << " acquired lock on "
                 << resource << "\n";

            return true;
        }

        cout << server << " failed to acquire lock on "
             << resource << "\n";

        return false;
    }

    // Release lock
    void releaseLock(string resource, string server) {

        auto it = locks.find(resource);

        if (it != locks.end() && it->second == server) {
            locks.erase(it);

            cout << server << " released lock on "
                 << resource << "\n";
        } else {
            cout << server << " cannot release this lock\n";
        }
    }

    // Display active locks
    void showLocks() {
        cout << "\nActive Locks:\n";

        if (locks.empty()) {
            cout << "No active locks\n";
            return;
        }

        for (auto& lock : locks) {
            cout << lock.first << " -> "
                 << lock.second << "\n";
        }
    }
};

int main() {
    DistributedLock lockManager;

    // Server A acquires lock
    lockManager.acquireLock("Movie_Ticket_101", "Server_A");

    // Server B tries to acquire the same lock
    lockManager.acquireLock("Movie_Ticket_101", "Server_B");

    lockManager.showLocks();

    cout << "\n";

    // Server A releases the lock
    lockManager.releaseLock("Movie_Ticket_101", "Server_A");

    // Server B tries again
    lockManager.acquireLock("Movie_Ticket_101", "Server_B");

    lockManager.showLocks();

    return 0;
}
