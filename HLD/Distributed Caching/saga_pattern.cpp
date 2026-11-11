#include <iostream>
#include <string>
using namespace std;

// Order Service
class OrderService {
public:
    bool createOrder() {
        cout << "Order Service: Creating order...\n";
        cout << "Order created successfully!\n";
        return true;
    }

    void cancelOrder() {
        cout << "Compensation: Order cancelled!\n";
    }
};

// Payment Service
class PaymentService {
public:
    bool processPayment() {
        cout << "Payment Service: Processing payment...\n";
        cout << "Payment successful!\n";
        return true;
    }

    void refundPayment() {
        cout << "Compensation: Payment refunded!\n";
    }
};

// Inventory Service
class InventoryService {
public:
    bool reserveProduct(bool shouldFail) {
        cout << "Inventory Service: Reserving product...\n";

        if (shouldFail) {
            cout << "Inventory reservation failed!\n";
            return false;
        }

        cout << "Product reserved successfully!\n";
        return true;
    }

    void releaseProduct() {
        cout << "Compensation: Product reservation released!\n";
    }
};

// Saga Orchestrator
class SagaOrchestrator {
private:
    OrderService order;
    PaymentService payment;
    InventoryService inventory;

public:
    void executeSaga(bool inventoryFailure) {

        cout << "\n--- Starting Saga Transaction ---\n";

        // Step 1: Create Order
        if (!order.createOrder()) {
            cout << "Saga failed!\n";
            return;
        }

        // Step 2: Process Payment
        if (!payment.processPayment()) {
            order.cancelOrder();
            cout << "Saga failed!\n";
            return;
        }

        // Step 3: Reserve Inventory
        if (!inventory.reserveProduct(inventoryFailure)) {

            cout << "\n--- Starting Compensation ---\n";

            // Reverse previous successful operations
            payment.refundPayment();
            order.cancelOrder();

            cout << "\nSaga transaction rolled back!\n";
            return;
        }

        cout << "\nSaga transaction completed successfully!\n";
    }
};

int main() {

    SagaOrchestrator saga;

    // Scenario 1: All operations succeed
    cout << "SCENARIO 1: SUCCESS\n";

    saga.executeSaga(false);

    // Scenario 2: Inventory operation fails
    cout << "\n\nSCENARIO 2: FAILURE\n";

    saga.executeSaga(true);

    return 0;
}
