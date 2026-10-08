#include <iostream>
#include <string>

using namespace std;


// -------------------------
// Message
// -------------------------
class Message {

public:
    string data;
    int retryCount;

    Message(string data) {
        this->data = data;
        retryCount = 0;
    }
};


// -------------------------
// Worker
// -------------------------
class Worker {

private:
    int maxRetries;

public:

    Worker(int maxRetries) {
        this->maxRetries = maxRetries;
    }


    void process(Message& message) {

        while (message.retryCount < maxRetries) {

            message.retryCount++;

            cout << "Attempt "
                 << message.retryCount
                 << ": Processing "
                 << message.data
                 << endl;


            // Simulate failure
            bool success = false;


            if (success) {

                cout << "Message processed successfully!\n";

                return;
            }


            cout << "Processing failed.\n";


            if (message.retryCount < maxRetries) {

                cout << "Retrying...\n\n";
            }
        }


        cout << "\nMaximum retries reached.\n";
        cout << "Message processing failed.\n";
    }
};


int main() {

    Message message("Process Payment");

    Worker worker(3);


    worker.process(message);


    return 0;
}