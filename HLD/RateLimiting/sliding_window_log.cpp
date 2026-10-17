#include <iostream>
#include <queue>
#include <string>

using namespace std;

class RateLimiter {
private:
    int requestLimit;
    int windowSize;
    queue<int> timestamps;

public:
    RateLimiter(int limit, int window) {
        requestLimit = limit;
        windowSize = window;
    }

    bool allowRequest(int currentTime) {
        // Remove requests outside the current window
        while (!timestamps.empty() &&
               timestamps.front() <= currentTime - windowSize) {
            timestamps.pop();
        }

        // Reject if the request limit is reached
        if (timestamps.size() >= requestLimit) {
            return false;
        }

        // Record this request
        timestamps.push(currentTime);
        return true;
    }
};

int main() {
    // Allow 3 requests in any 10-second window
    RateLimiter limiter(3, 10);

    int requestTimes[] = {2, 5, 8, 9, 12, 16};

    for (int i = 0; i < 6; i++) {
        int time = requestTimes[i];

        cout << "Time " << time << ": ";

        if (limiter.allowRequest(time)) {
            cout << "Request Allowed\n";
        } else {
            cout << "Request Rejected (HTTP 429)\n";
        }
    }

    return 0;
}
