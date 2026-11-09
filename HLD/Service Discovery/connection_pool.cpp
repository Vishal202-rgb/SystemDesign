#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Connection {
public:
    int id;
    bool inUse;

    Connection(int connectionId) {
        id = connectionId;
        inUse = false;
    }
};

class ConnectionPool {
private:
    vector<Connection> connections;
    int maxPoolSize;
    int nextId;

public:
    ConnectionPool(int maxSize) {
        maxPoolSize = maxSize;
        nextId = 1;
    }

    // Acquire a connection
    Connection* acquireConnection() {

        // Find an available connection
        for (auto& connection : connections) {
            if (!connection.inUse) {
                connection.inUse = true;

                cout << "Connection "
                     << connection.id
                     << " acquired\n";

                return &connection;
            }
        }

        // Create a new connection if pool has space
        if (connections.size() < maxPoolSize) {

            connections.emplace_back(nextId);
            nextId++;

            connections.back().inUse = true;

            cout << "New Connection "
                 << connections.back().id
                 << " created and acquired\n";

            return &connections.back();
        }

        cout << "No connection available! Please wait.\n";
        return nullptr;
    }

    // Release a connection
    void releaseConnection(Connection* connection) {

        if (connection == nullptr) {
            return;
        }

        if (connection->inUse) {
            connection->inUse = false;

            cout << "Connection "
                 << connection->id
                 << " released\n";
        }
    }

    // Display pool status
    void showPoolStatus() {

        cout << "\nConnection Pool Status:\n";

        cout << "Total Connections: "
             << connections.size() << "\n";

        int available = 0;

        for (auto& connection : connections) {
            if (!connection.inUse) {
                available++;
            }
        }

        cout << "Available Connections: "
             << available << "\n";

        cout << "Busy Connections: "
             << connections.size() - available << "\n";
    }
};

int main() {

    ConnectionPool pool(3);

    cout << "--- Acquiring Connections ---\n";

    Connection* c1 = pool.acquireConnection();
    Connection* c2 = pool.acquireConnection();
    Connection* c3 = pool.acquireConnection();

    pool.showPoolStatus();

    cout << "\n--- Trying Extra Connection ---\n";

    Connection* c4 = pool.acquireConnection();

    cout << "\n--- Releasing Connection ---\n";

    pool.releaseConnection(c2);

    cout << "\n--- Acquiring Again ---\n";

    c4 = pool.acquireConnection();

    pool.showPoolStatus();

    return 0;
}
