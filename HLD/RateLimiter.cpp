#include <iostream>
#include <unordered_map>
using namespace std;

class RateLimiter {

    unordered_map<int, int> requests;

    int limit = 5;

public:

    bool allowRequest(int userId) {

        if (requests[userId] >= limit) {
            return false;
        }

        requests[userId]++;

        return true;
    }
};

int main() {

    RateLimiter limiter;

    for (int i = 0; i < 7; i++) {

        if (limiter.allowRequest(101))
            cout << "Request Allowed\n";
        else
            cout << "Request Blocked\n";
    }
}