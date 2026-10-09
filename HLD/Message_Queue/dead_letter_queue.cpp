#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct Message {
    string data;
    int attempts = 0;
};

class MessageQueue {
private:
    queue<Message> messages;
    queue<Message> deadLetterQueue;
    int maxRetries = 3;

public:
    // Add a message to the main queue
    void send(string data) {
        messages.push({data, 0});
    }

    // Process messages
    void processMessages() {
        while (!messages.empty()) {
            Message msg = messages.front();
            messages.pop();

            bool success = false; // Simulate failure

            while (msg.attempts < maxRetries) {
                msg.attempts++;

                cout << "Attempt " << msg.attempts
                     << ": " << msg.data << endl;

                // Simulate success on attempt 3
                if (msg.data == "Send Email"
                    && msg.attempts == 3) {
                    success = true;
                }

                if (success) {
                    cout << "Message processed successfully!\n\n";
                    break;
                }

                cout << "Processing failed!\n";
            }

            // Move repeatedly failed messages to DLQ
            if (!success) {
                deadLetterQueue.push(msg);
                cout << "Moved to Dead Letter Queue: "
                     << msg.data << "\n\n";
            }
        }
    }

    // Display failed messages
    void showDeadLetterQueue() {
        cout << "\n--- Dead Letter Queue ---\n";

        if (deadLetterQueue.empty()) {
            cout << "No failed messages.\n";
            return;
        }

        while (!deadLetterQueue.empty()) {
            Message msg = deadLetterQueue.front();
            deadLetterQueue.pop();

            cout << msg.data
                 << " | Attempts: " << msg.attempts
                 << endl;
        }
    }
};

int main() {
    MessageQueue queue;

    queue.send("Process Payment");
    queue.send("Send Email");
    queue.send("Generate Invoice");

    queue.processMessages();
    queue.showDeadLetterQueue();

    return 0;
}
