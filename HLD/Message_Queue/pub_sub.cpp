
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Subscriber
class Subscriber {
private:
    string name;

public:
    Subscriber(string name) {
        this->name = name;
    }

    void receiveMessage(string message) {
        cout << name << " received: "
             << message << endl;
    }
};

// Topic / Message Broker
class Topic {
private:
    vector<Subscriber*> subscribers;

public:
    // Subscribe to the topic
    void subscribe(Subscriber* subscriber) {
        subscribers.push_back(subscriber);
    }

    // Publish message to all subscribers
    void publish(string message) {
        cout << "\nPublishing: " << message << endl;

        for (Subscriber* subscriber : subscribers) {
            subscriber->receiveMessage(message);
        }
    }
};

int main() {
    // Create subscribers
    Subscriber paymentService("Payment Service");
    Subscriber emailService("Email Service");
    Subscriber analyticsService("Analytics Service");

    // Create topic
    Topic orderTopic;

    // Subscribe services to the topic
    orderTopic.subscribe(&paymentService);
    orderTopic.subscribe(&emailService);
    orderTopic.subscribe(&analyticsService);

    // Publisher sends an event
    orderTopic.publish("Order Created");

    return 0;
}
