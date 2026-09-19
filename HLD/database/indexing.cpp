#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;


// --------------------
// User
// --------------------
struct User {

    int id;
    string name;
    string email;
};


// --------------------
// Database
// --------------------
class Database {

private:

    vector<User> users;

    // Index:
    // email -> position in users vector
    unordered_map<string, int> emailIndex;


public:

    // Insert user
    void addUser(int id, string name, string email) {

        users.push_back({id, name, email});

        // Create index
        emailIndex[email] = users.size() - 1;
    }


    // Search without index
    User* findWithoutIndex(string email) {

        cout << "Searching without index...\n";

        for (auto &user : users) {

            if (user.email == email) {
                return &user;
            }
        }

        return nullptr;
    }


    // Search using index
    User* findWithIndex(string email) {

        cout << "Searching using index...\n";

        if (emailIndex.find(email) == emailIndex.end()) {
            return nullptr;
        }

        int position = emailIndex[email];

        return &users[position];
    }
};


int main() {

    Database db;


    db.addUser(
        1,
        "Vishal",
        "vishal@gmail.com"
    );

    db.addUser(
        2,
        "Rahul",
        "rahul@gmail.com"
    );

    db.addUser(
        3,
        "Aman",
        "aman@gmail.com"
    );


    // -------------------------
    // Without Index
    // -------------------------

    User* user1 =
        db.findWithoutIndex(
            "aman@gmail.com"
        );


    if (user1 != nullptr) {

        cout << user1->name << endl;
    }


    // -------------------------
    // With Index
    // -------------------------

    User* user2 =
        db.findWithIndex(
            "aman@gmail.com"
        );


    if (user2 != nullptr) {

        cout << user2->name << endl;
    }


    return 0;
}