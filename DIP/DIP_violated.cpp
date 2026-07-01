#include <iostream>
using namespace std;

//The second code violates the Dependency Inversion Principle because the high-level module UserService directly depends on concrete implementations (MySQLDatabase and MongoDBDatabase). If a new database is introduced, UserService must be modified, making it tightly coupled.
class MySQLDatabase {  // Low-level module
public:
    void saveToSQL(string data) {
        cout << "Executing SQL Query: INSERT INTO users VALUES('" << data << "');" << endl;
    }
};

class MongoDBDatabase {  // Low-level module
public:
    void saveToMongo(string data) {
        cout << "Executing MongoDB Function: db.users.insert({name: '" << data << "'})" << endl;
    }
};

class UserService {  // High-level module (Tightly coupled)
private:
    MySQLDatabase sqlDb;  // Direct dependency on MySQL
    MongoDBDatabase mongoDb;  // Direct dependency on MongoDB

public:
    void storeUserToSQL(string user) {
        // MySQL-specific code
        sqlDb.saveToSQL(user);  
    }

    void storeUserToMongo(string user) {
        // MongoDB-specific code
        mongoDb.saveToMongo(user);  
    }
};

int main() {
    UserService service;
    service.storeUserToSQL("Aditya");
    service.storeUserToMongo("Rohit");
}
/*
S → Single Job (One class, one responsibility)
O → Open to Extend (Add new classes, don't modify old ones)
L → Legal Substitute (Child must behave like parent)
I → Implement Only What's Needed (Small, focused interfaces)
D → Depend on Interfaces, Not Implementations (Use abstraction and dependency injection)
*/