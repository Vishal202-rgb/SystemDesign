#include <iostream>
#include <queue>
using namespace std;

class MessageQueue {

    queue<string> messages;

public:

    void publish(string message) {
        messages.push(message);
    }

    string consume() {

        if (messages.empty())
            return "";

        string message = messages.front();

        messages.pop();

        return message;
    }
};

int main() {

    MessageQueue mq;

    mq.publish("Order #101");
    mq.publish("Order #102");
    mq.publish("Order #103");

    cout << mq.consume() << endl;
    cout << mq.consume() << endl;
    cout << mq.consume() << endl;
}
/*
Order Service
     ↓
Message Queue
     ↓
Worker
     ↓
Email / Payment / Notification
*/