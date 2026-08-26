#include <iostream>
#include <vector>
using namespace std;

class Server {
public:

    int id;

    Server(int id) {
        this->id = id;
    }

    void handleRequest() {
        cout << "Request handled by Server " << id << endl;
    }
     void setHealth(bool status) {
        healthy = status;
    }
private:
    bool healthy = true;
};

 /*                 Load Balancer
                       |
          ┌────────────┼────────────┐
          ↓            ↓            ↓
       Server 1     Server 2     Server 3

                        WITH LB

10,000 users
     |
     ↓
LOAD BALANCER
  /    |    \
 ↓     ↓     ↓
S1     S2     S3
 */
class LoadBalancer {
private:
    vector<Server*> servers;
    int index = 0;

public:
    void addServer(Server* server) {
        servers.push_back(server);
    }

    Server* routeRequest() {
        if (servers.empty()) {
            return nullptr;
        }

        Server* selected = servers[index];

        index = (index + 1) % servers.size();

        return selected;
    }
};

int main() {

    Server s1(1);
    Server s2(2);
    Server s3(3);

    LoadBalancer lb;

    lb.addServer(&s1);
    lb.addServer(&s2);
    lb.addServer(&s3);

    for (int i = 0; i < 6; i++) {

        Server* server = lb.routeRequest();

        server->handleRequest();
    }
     cout << "\n----- SERVER 2 DOWN -----\n";

    s2.setHealth(false);


    for (int i = 0; i < 6; i++) {

        Server* server = lb.routeRequest();

        if (server != nullptr) {
            server->handleRequest();
        }
        else {
            cout << "No healthy server available\n";
        }
    }

    return 0;
}

/*Client
  ↓
CDN
  ↓
API Gateway
  ↓
Load Balancer
  ↓
┌──────────┬──────────┬──────────┐
Auth       User       Order
Service    Service    Service
   └──────────┬──────────┘
              ↓
          Message Queue
              ↓
          Workers
              ↓
          Database
*/