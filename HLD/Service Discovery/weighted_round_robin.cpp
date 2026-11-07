#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

// Represents an individual server
struct Server {
    string address;
    bool healthy;

    Server(string addr) {
        address = addr;
        healthy = true;
    }
};

// Service Discovery
class ServiceRegistry {
private:
    unordered_map<string, vector<Server>> services;

public:
    // Register a server
    void registerServer(string serviceName, string address) {
        services[serviceName].push_back(Server(address));

        cout << "Registered: " << address
             << " for " << serviceName << "\n";
    }

    // Get healthy servers
    vector<Server>& discoverServers(string serviceName) {
        return services[serviceName];
    }

    // Update server health
    void updateHealth(string serviceName,
                      string address,
                      bool status) {

        for (auto& server : services[serviceName]) {
            if (server.address == address) {
                server.healthy = status;

                cout << address << " is now "
                     << (status ? "HEALTHY" : "UNHEALTHY")
                     << "\n";
                return;
            }
        }
    }
};

// Round Robin Load Balancer
class LoadBalancer {
private:
    unordered_map<string, int> currentIndex;

public:
    string getServer(string serviceName,
                     vector<Server>& servers) {

        vector<int> healthyServers;

        for (int i = 0; i < (int)servers.size(); i++) {
            if (servers[i].healthy) {
                healthyServers.push_back(i);
            }
        }

        if (healthyServers.empty()) {
            return "No healthy servers available";
        }

        int& index = currentIndex[serviceName];

        int selectedIndex =
            healthyServers[index % healthyServers.size()];

        index = (index + 1) % healthyServers.size();

        return servers[selectedIndex].address;
    }
};

int main() {

    ServiceRegistry registry;
    LoadBalancer balancer;

    // Register Payment Service instances
    cout << "\n--- Service Registration ---\n";

    registry.registerServer("PaymentService", "10.0.0.1:5001");
    registry.registerServer("PaymentService", "10.0.0.2:5002");
    registry.registerServer("PaymentService", "10.0.0.3:5003");

    // Discover and distribute requests
    cout << "\n--- Load Balancing ---\n";

    for (int i = 1; i <= 6; i++) {

        vector<Server>& servers =
            registry.discoverServers("PaymentService");

        string selected =
            balancer.getServer("PaymentService", servers);

        cout << "Request " << i
             << " -> " << selected << "\n";
    }

    // Simulate server failure
    cout << "\n--- Health Check ---\n";

    registry.updateHealth(
        "PaymentService",
        "10.0.0.2:5002",
        false
    );

    // Distribute requests again
    cout << "\n--- After Server Failure ---\n";

    for (int i = 1; i <= 6; i++) {

        vector<Server>& servers =
            registry.discoverServers("PaymentService");

        string selected =
            balancer.getServer("PaymentService", servers);

        cout << "Request " << i
             << " -> " << selected << "\n";
    }

    return 0;
}
