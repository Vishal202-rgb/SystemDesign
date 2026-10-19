#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class RateLimiter {
private:
    int requestLimit;
    int windowSize;

    struct UserData {
        int currentWindow = -1;
        int currentCount = 0;
        int previousCount = 0;
    };

    unordered_map<string, UserData> users;

public:
    RateLimiter(int limit, int window) {
        requestLimit = limit;
        windowSize = window;
    }

    bool allowRequest(string userId, int currentTime) {
        int window = currentTime / windowSize;
        int elapsed = currentTime % windowSize;

        UserData& user = users[userId];

        // Move to a new window
        if (user.currentWindow != window) {
            int difference = window - user.currentWindow;

            if (difference == 1) {
                user.previousCount = user.currentCount;
            } else {
                user.previousCount = 0;
            }

            user.currentCount = 0;
            user.currentWindow = window;
        }

        // Estimate previous-window requests still relevant
        double previousWeight =
            (double)(windowSize - elapsed) / windowSize;

        double estimatedCount =
            user.previousCount * previousWeight
            + user.currentCount;

        if (estimatedCount >= requestLimit) {
            return false;
        }

        user.currentCount++;
        return true;
    }
};

int main() {
    // Allow approximately 5 requests per 10-second window
    RateLimiter limiter(5, 10);

    string user = "Vishal";

    int requestTimes[] = {
        2, 4, 6, 8, 9, 10, 11, 12, 15
    };

    for (int time : requestTimes) {
        cout << "Time " << time << ": ";

        if (limiter.allowRequest(user, time)) {
            cout << "Request Allowed\n";
        } else {
            cout << "Request Rejected (HTTP 429)\n";
        }
    }

    return 0;
}
