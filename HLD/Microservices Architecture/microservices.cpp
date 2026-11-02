
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

// User Service
class UserService {
public:
    void getUser(string userId) {
        cout << "User Service: Fetching user "
             << userId << "\n";
    }
};

// Product Service
class ProductService {
public:
    void getProduct(string productId) {
        cout << "Product Service: Fetching product "
             << productId << "\n";
    }
};

// Payment Service
class PaymentService {
public:
    bool processPayment(double amount) {
        cout << "Payment Service: Processing payment of Rs. "
             << amount << "\n";

        cout << "Payment successful!\n";
        return true;
    }
};

// Order Service
class OrderService {
private:
    PaymentService& paymentService;

public:
    OrderService(PaymentService& payment) 
        : paymentService(payment) {}

    void createOrder(string orderId, double amount) {
        cout << "Order Service: Creating order "
             << orderId << "\n";

        if (paymentService.processPayment(amount)) {
            cout << "Order created successfully!\n";
        } else {
            cout << "Order failed!\n";
        }
    }
};

// API Gateway
class APIGateway {
private:
    UserService userService;
    ProductService productService;
    PaymentService paymentService;
    OrderService orderService;

public:
    APIGateway()
        : orderService(paymentService) {}

    void handleRequest(string endpoint) {

        cout << "\nAPI Gateway received: "
             << endpoint << "\n";

        if (endpoint == "/users") {
            userService.getUser("101");
        }
        else if (endpoint == "/products") {
            productService.getProduct("P101");
        }
        else if (endpoint == "/orders") {
            orderService.createOrder("O101", 1500);
        }
        else {
            cout << "404: Service not found\n";
        }
    }
};

int main() {

    APIGateway gateway;

    gateway.handleRequest("/users");
    gateway.handleRequest("/products");
    gateway.handleRequest("/orders");
    gateway.handleRequest("/unknown");

    return 0;
}
