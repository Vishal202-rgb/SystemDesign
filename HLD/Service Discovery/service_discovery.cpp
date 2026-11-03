#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class ServiceRegistry {
private:
    unordered_map<string, string> services;

public:
    // Register a service
    void registerService(string serviceName, string address) {
        services[serviceName] = address;

        cout << serviceName
             << " registered at "
             << address << "\n";
    }

    // Discover a service
    void discoverService(string serviceName) {

        auto it = services.find(serviceName);

        if (it != services.end()) {
            cout << "Service Found: " << serviceName << "\n";
            cout << "Address: " << it->second << "\n";
        } else {
            cout << "Service Not Found: "
                 << serviceName << "\n";
        }
    }

    // Remove a service
    void removeService(string serviceName) {

        if (services.erase(serviceName)) {
            cout << serviceName
                 << " removed from registry\n";
        } else {
            cout << serviceName
                 << " does not exist\n";
        }
    }

    // Display all registered services
    void showServices() {

        cout << "\nRegistered Services:\n";

        for (auto& service : services) {
            cout << service.first
                 << " -> " << service.second << "\n";
        }
    }
};

int main() {

    ServiceRegistry registry;

    // Register services
    registry.registerService("UserService", "10.0.0.1:5001");
    registry.registerService("OrderService", "10.0.0.2:5002");
    registry.registerService("PaymentService", "10.0.0.3:5003");

    cout << "\n";

    // Discover services
    registry.discoverService("PaymentService");
    registry.discoverService("NotificationService");

    // Display registered services
    registry.showServices();

    cout << "\n";

    // Remove a service
    registry.removeService("PaymentService");

    // Try discovering removed service
    registry.discoverService("PaymentService");

    return 0;
}
