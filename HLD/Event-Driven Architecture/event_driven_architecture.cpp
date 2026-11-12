#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
using namespace std;

// Event structure
struct Event {
    string eventName;
    string message;
};

// Event Broker
class EventBroker {
private:
    unordered_map<string, vector<function<void(Event)>>> subscribers;

public:
    // Subscribe to an event
    void subscribe(
        string eventName,
        function<void(Event)> callback
    ) {
        subscribers[eventName].push_back(callback);

        cout << "Subscribed to: "
             << eventName << "\n";
    }

    // Publish an event
    void publish(Event event) {
        cout << "\nEvent Published: "
             << event.eventName << "\n";

        if (subscribers.find(event.eventName)
            == subscribers.end()) {

            cout << "No subscribers found!\n";
            return;
        }

        // Notify all subscribers
        for (auto& callback : subscribers[event.eventName]) {
            callback(event);
        }
    }
};

// Order Service - Producer
class OrderService {
private:
    EventBroker& broker;

public:
    OrderService(EventBroker& b) : broker(b) {}

    void createOrder() {
        cout << "\nOrder created successfully!\n";

        Event event = {
            "OrderCreated",
            "Order O101 created"
        };

        broker.publish(event);
    }
};

// Email Service - Consumer
class EmailService {
public:
    void handleEvent(Event event) {
        cout << "Email Service: Sending confirmation email for "
             << event.message << "\n";
    }
};

// Inventory Service - Consumer
class InventoryService {
public:
    void handleEvent(Event event) {
        cout << "Inventory Service: Updating stock for "
             << event.message << "\n";
    }
};

// Analytics Service - Consumer
class AnalyticsService {
public:
    void handleEvent(Event event) {
        cout << "Analytics Service: Recording event "
             << event.eventName << "\n";
    }
};

int main() {

    EventBroker broker;

    OrderService orderService(broker);

    EmailService emailService;
    InventoryService inventoryService;
    AnalyticsService analyticsService;

    // Subscribe consumers to OrderCreated event
    broker.subscribe(
        "OrderCreated",
        [&](Event e) {
            emailService.handleEvent(e);
        }
    );

    broker.subscribe(
        "OrderCreated",
        [&](Event e) {
            inventoryService.handleEvent(e);
        }
    );

    broker.subscribe(
        "OrderCreated",
        [&](Event e) {
            analyticsService.handleEvent(e);
        }
    );

    // Producer publishes event
    orderService.createOrder();

    return 0;
}