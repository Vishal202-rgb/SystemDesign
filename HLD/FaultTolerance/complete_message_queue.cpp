
#include <iostream>
#include <queue>
#include <string>
using namespace std;

// Message structure
struct Message {
    int id;
    string content;
    int retryCount;
};

// Message Queue System
class MessageQueue {
private:
    queue<Message> messages;
    queue<Message> deadLetterQueue;

    int maxRetries;

public:
    MessageQueue(int retries) {
        maxRetries = retries;
    }

    // Producer sends message
    void produce(int id, string content) {
        Message msg = {id, content, 0};

        messages.push(msg);

        cout << "Message Produced: " << content << "\n";
    }

    // Consumer processes message
    void consume(bool processingSuccess) {

        if (messages.empty()) {
            cout << "Queue is empty!\n";
            return;
        }

        Message msg = messages.front();
        messages.pop();

        cout << "\nProcessing Message: " << msg.content << "\n";

        if (processingSuccess) {

            // ACK: Processing successful
            cout << "ACK: Message processed successfully\n";

        } else {

            // Processing failed
            cout << "Processing Failed!\n";

            msg.retryCount++;

            if (msg.retryCount <= maxRetries) {

                cout << "Retry Attempt: "
                     << msg.retryCount << "\n";

                messages.push(msg);

            } else {

                // Move message to DLQ
                cout << "Max retries reached!\n";
                cout << "Moving message to Dead Letter Queue\n";

                deadLetterQueue.push(msg);
            }
        }
    }

    void showQueues() {

        cout << "\nPending Messages: "
             << messages.size() << "\n";

        cout << "Dead Letter Queue: "
             << deadLetterQueue.size() << "\n";
    }
};

int main() {

    MessageQueue mq(2);

    // Producer
    mq.produce(1, "Order Created");
    mq.produce(2, "Payment Processed");

    mq.showQueues();

    // Consumer
    mq.consume(false);  // First attempt fails
    mq.consume(true);   // Second message succeeds

    mq.showQueues();

    mq.consume(false);  // Retry attempt 1
    mq.consume(false);  // Retry attempt 2
    mq.consume(false);  // Moves to DLQ

    mq.showQueues();

    return 0;
}
