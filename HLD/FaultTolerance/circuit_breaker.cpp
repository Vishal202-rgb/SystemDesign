#include <iostream>
#include <string>
using namespace std;

class CircuitBreaker {
private:
    string state;
    int failureCount;
    int failureThreshold;
    int resetTimeout;
    int lastFailureTime;

public:
    CircuitBreaker(int threshold, int timeout) {
        state = "CLOSED";
        failureCount = 0;
        failureThreshold = threshold;
        resetTimeout = timeout;
        lastFailureTime = 0;
    }

    bool allowRequest(int currentTime) {

        // Check whether OPEN state can move to HALF-OPEN
        if (state == "OPEN") {
            if (currentTime - lastFailureTime >= resetTimeout) {
                state = "HALF-OPEN";
                cout << "Circuit changed to HALF-OPEN\n";
            } else {
                cout << "Request blocked! Circuit is OPEN\n";
                return false;
            }
        }

        return true;
    }

    void recordSuccess() {
        cout << "Request successful!\n";

        failureCount = 0;
        state = "CLOSED";
    }

    void recordFailure(int currentTime) {
        cout << "Request failed!\n";

        failureCount++;
        lastFailureTime = currentTime;

        if (failureCount >= failureThreshold) {
            state = "OPEN";
            cout << "Circuit changed to OPEN\n";
        }
    }

    void showState() {
        cout << "Current State: " << state << "\n";
    }
};

int main() {
    CircuitBreaker breaker(3, 5);

    // First request fails
    for (int i = 1; i <= 3; i++) {
        cout << "\nRequest " << i << "\n";

        if (breaker.allowRequest(i)) {
            breaker.recordFailure(i);
        }

        breaker.showState();
    }

    // Requests are blocked
    cout << "\nRequest 4\n";
    breaker.allowRequest(4);

    // After timeout, a test request is allowed
    cout << "\nRequest 5 (After Timeout)\n";

    if (breaker.allowRequest(8)) {
        breaker.recordSuccess();
    }

    breaker.showState();

    return 0;
}
