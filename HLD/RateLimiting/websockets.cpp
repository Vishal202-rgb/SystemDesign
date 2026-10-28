#include <iostream>
#include <string>
#include <vector>
using namespace std;

class WebSocketServer {
private:
    vector<string> clients;

public:
    void connectClient(string username) {
        clients.push_back(username);
        cout << username << " connected to server.\n";
    }

    void sendMessage(string username, string message) {
        cout << username << " sent: " << message << "\n";

        // Broadcast message to connected clients
        for (const string& client : clients) {
            if (client != username) {
                cout << "Message delivered to "
                     << client << "\n";
            }
        }
    }

    void disconnectClient(string username) {
        for (auto it = clients.begin();
             it != clients.end(); ++it) {
            if (*it == username) {
                clients.erase(it);
                cout << username << " disconnected.\n";
                return;
            }
        }
    }
};
int main() {
    WebSocketServer server;

    server.connectClient("Vishal");
    server.connectClient("Rahul");

    cout << "\n";

    server.sendMessage("Vishal", "Hello Rahul!");

    cout << "\n";

    server.disconnectClient("Vishal");

    return 0;
}
