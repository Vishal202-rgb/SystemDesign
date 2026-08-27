#include <iostream>
#include <vector>
using namespace std;

class Server {

public:
    int id;
    int connections;

    Server(int id) {
        this->id = id;
        connections = 0;
    }

    void handleRequest() {
        connections++;

        cout << "Request → Server "
             << id
             << " | Connections = "
             << connections << endl;
    }
};

class LoadBalancer {

    vector<Server*> servers;

public:

    void addServer(Server* server) {
        servers.push_back(server);
    }

    Server* getServer() {

        Server* best = servers[0];

        for (Server* server : servers) {

            if (server->connections < best->connections) {
                best = server;
            }
        }

        return best;
    }
};