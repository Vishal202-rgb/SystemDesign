#include <iostream>
#include <algorithm>
using namespace std;

class LeakyBucket {
private:
    double capacity;
    double water;
    double leakRate;
    double lastTime;

public:
    LeakyBucket(double cap, double rate) {
        capacity = cap;
        water = 0;
        leakRate = rate;
        lastTime = 0;
    }

    bool allowRequest(double currentTime) {
        // Leak water according to elapsed time
        double elapsed = currentTime - lastTime;
        water = max(0.0, water - elapsed * leakRate);
        lastTime = currentTime;

        // Reject if the bucket is full
        if (water + 1 > capacity) {
            return false;
        }

        // Each request adds one unit of water
        water += 1;
        return true;
    }
};

int main() {
    LeakyBucket limiter(3, 1);

    double requestTimes[] = {0, 0, 0, 0, 1, 1, 2};

    for (double time : requestTimes) {
        cout << "Time " << time << ": ";

        if (limiter.allowRequest(time)) {
            cout << "Request Allowed\n";
        } else {
            cout << "Request Rejected (HTTP 429)\n";
        }
    }

    return 0;
}