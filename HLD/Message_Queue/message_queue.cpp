#include <iostream>
#include <queue>
#include <string>

using namespace std;


// -------------------------
// Message Queue
// -------------------------
class MessageQueue {

private:
    queue<string> messages;

public:

    // Add message to queue
    void send(string message) {

        messages.push(message);

        cout << "Message added: "
             << message << endl;
    }


    // Get message from queue
    string receive() {

        if (messages.empty()) {
            return "Queue is empty";
        }

        string message = messages.front();

        messages.pop();

        return message;
    }
};


// -------------------------
// Producer
// -------------------------
class Producer {

private:
    MessageQueue& queue;

public:

    Producer(MessageQueue& q)
        : queue(q) {
    }

    void sendMessage(string message) {

        queue.send(message);
    }
};


// -------------------------
// Consumer
// -------------------------
class Consumer {

private:
    MessageQueue& queue;

public:

    Consumer(MessageQueue& q)
        : queue(q) {
    }

    void processMessage() {

        string message = queue.receive();

        if (message != "Queue is empty") {

            cout << "Consumer processed: "
                 << message << endl;
        }
    }
};


int main() {

    MessageQueue queue;

    Producer producer(queue);
    Consumer consumer(queue);


    // Producer sends messages
    producer.sendMessage("Order Created");
    producer.sendMessage("Payment Required");
    producer.sendMessage("Send Email");


    cout << "\nConsumer processing:\n";


    // Consumer processes messages
    consumer.processMessage();
    consumer.processMessage();
    consumer.processMessage();


    return 0;
}