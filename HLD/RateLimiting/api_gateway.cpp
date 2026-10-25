#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class APIGateway {
private:
    unordered_map<string, string> routes;

public:
    APIGateway() {
        routes["/users"] = "User Service";
        routes["/products"] = "Product Service";
        routes["/orders"] = "Order Service";
        routes["/payments"] = "Payment Service";
    }

    void handleRequest(string endpoint) {
        cout << "Request received: " << endpoint << "\n";

        auto it = routes.find(endpoint);

        if (it != routes.end()) {
            cout << "Routing to: " << it->second << "\n";
            cout << "Response: Request processed successfully\n";
        } else {
            cout << "Response: 404 Not Found\n";
        }

        cout << "-------------------------\n";
    }
};

int main() {
    APIGateway gateway;

    gateway.handleRequest("/users");
    gateway.handleRequest("/orders");
    gateway.handleRequest("/payments");
    gateway.handleRequest("/unknown");

    return 0;
}
