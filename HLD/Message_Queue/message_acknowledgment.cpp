#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct Message {
    int id;
    string data;
};

class MessageQueue {
private:
    queue<Message> messages;

public:
    void send(Message msg) {
        messages.push(msg);
    }

    Message getMessage() {
        Message msg = messages.front();
        messages.pop();
        return msg;
    }

    bool isEmpty() {
        return messages.empty();
    }

    void retry(Message msg) {
        messages.push(msg);
    }
};

class Consumer {
private:
    MessageQueue& queue;

public:
    Consumer(MessageQueue& q) : queue(q) {}

    void processMessage(bool shouldSucceed) {
        if (queue.isEmpty()) {
            cout << "No messages available.\n";
            return;
        }

        Message msg = queue.getMessage();

        cout << "\nProcessing: " << msg.data << endl;

        if (shouldSucceed) {
            // ACK: message processed successfully
            cout << "ACK received. Message completed.\n";
        } else {
            // No ACK: retry the message
            cout << "Processing failed. No ACK.\n";
            cout << "Message returned to queue for retry.\n";

            queue.retry(msg);
        }
    }
};

int main() {
    MessageQueue queue;

    queue.send({1, "Process Payment"});
    queue.send({2, "Send Email"});

    Consumer consumer(queue);

    // First message succeeds
    consumer.processMessage(true);

    // Second message fails
    consumer.processMessage(false);

    // Retry the second message; now it succeeds
    consumer.processMessage(true);

    return 0;
}
