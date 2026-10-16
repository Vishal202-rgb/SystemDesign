#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class RateLimiter {
private:
    int requestLimit;
    int windowSize;

    // Stores request count and window number for each user
    struct UserData {
        int requestCount = 0;
        int windowNumber = -1;
    };

    unordered_map<string, UserData> users;

public:
    RateLimiter(int limit, int window) {
        requestLimit = limit;
        windowSize = window;
    }

    bool allowRequest(string userId, int currentTime) {
        int currentWindow = currentTime / windowSize;

        UserData& user = users[userId];

        // Start a new window when the old one expires
        if (user.windowNumber != currentWindow) {
            user.requestCount = 0;
            user.windowNumber = currentWindow;
        }

        // Reject if the limit has been reached
        if (user.requestCount >= requestLimit) {
            return false;
        }

        user.requestCount++;
        return true;
    }
};

int main() {
    // Allow 3 requests per 10-second window
    RateLimiter limiter(3, 10);

    string user = "Vishal";

    for (int i = 1; i <= 5; i++) {
        if (limiter.allowRequest(user, 5)) {
            cout << "Request " << i << ": Allowed\n";
        } else {
            cout << "Request " << i << ": Rejected (HTTP 429)\n";
        }
    }

    cout << "\nAfter the window changes:\n";

    if (limiter.allowRequest(user, 10)) {
        cout << "New request: Allowed\n";
    } else {
        cout << "New request: Rejected\n";
    }

    return 0;
}
