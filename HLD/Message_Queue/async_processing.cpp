#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <chrono>

using namespace std;


// -------------------------
// Message Queue
// -------------------------
class MessageQueue {

private:
    queue<string> tasks;

public:

    void addTask(string task) {
        tasks.push(task);
    }

    bool hasTask() {
        return !tasks.empty();
    }

    string getTask() {

        string task = tasks.front();
        tasks.pop();

        return task;
    }
};


// -------------------------
// Background Worker
// -------------------------
void worker(MessageQueue& queue) {

    while (queue.hasTask()) {

        string task = queue.getTask();

        cout << "Worker processing: "
             << task << endl;

        // Simulate time-consuming work
        this_thread::sleep_for(
            chrono::seconds(1)
        );

        cout << "Completed: "
             << task << endl;
    }
}


// -------------------------
// Main
// -------------------------
int main() {

    MessageQueue queue;


    // User sends tasks
    cout << "User created an order.\n";

    queue.addTask("Process Payment");
    queue.addTask("Send Email");
    queue.addTask("Generate Invoice");


    // Start background worker
    thread backgroundWorker(
        worker,
        ref(queue)
    );


    // User does not wait for every task
    cout << "\nUser received response immediately!\n";


    // Wait for worker to finish
    backgroundWorker.join();


    cout << "\nAll background tasks completed.\n";


    return 0;
}