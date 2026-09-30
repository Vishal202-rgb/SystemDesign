#include <iostream>

using namespace std;

class DistributedSystem {

private:
    bool networkPartition;

public:

    DistributedSystem(bool partition) {
        networkPartition = partition;
    }

    void handleRequest() {

        if (!networkPartition) {

            cout << "Network is working.\n";
            cout << "Consistency + Availability maintained.\n";

            return;
        }

        cout << "Network Partition detected!\n\n";

        cout << "CP System:\n";
        cout << "Choose Consistency.\n";
        cout << "Request may be rejected until data is synchronized.\n\n";


        cout << "AP System:\n";
        cout << "Choose Availability.\n";
        cout << "Request is served even if data may be temporarily inconsistent.\n";
    }
};


int main() {

    // Simulate a network failure
    DistributedSystem system(true);

    system.handleRequest();

    return 0;
}