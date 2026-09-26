#include <iostream>
#include <map>
#include <string>

using namespace std;


// -------------------------
// SQL Database
// -------------------------
class SQLDatabase {

private:
    // Fixed table structure
    map<int, pair<string, string>> users;

public:

    void insert(int id, string name, string email) {
        users[id] = {name, email};
    }

    void showData() {

        cout << "\nSQL Database\n";
        cout << "------------------\n";

        cout << "ID\tName\tEmail\n";

        for (auto &user : users) {

            cout << user.first << "\t"
                 << user.second.first << "\t"
                 << user.second.second << endl;
        }
    }
};


// -------------------------
// NoSQL Database
// -------------------------
class NoSQLDatabase {

private:
    // Flexible document structure
    map<int, map<string, string>> users;

public:

    void insert(int id, map<string, string> document) {
        users[id] = document;
    }

    void showData() {

        cout << "\nNoSQL Database\n";
        cout << "------------------\n";

        for (auto &user : users) {

            cout << "ID: " << user.first << endl;

            for (auto &field : user.second) {

                cout << field.first
                     << ": "
                     << field.second
                     << endl;
            }

            cout << endl;
        }
    }
};


int main() {

    // =========================
    // SQL
    // =========================

    SQLDatabase sql;

    sql.insert(
        1,
        "Vishal",
        "vishal@gmail.com"
    );

    sql.insert(
        2,
        "Rahul",
        "rahul@gmail.com"
    );

    sql.showData();


    // =========================
    // NoSQL
    // =========================

    NoSQLDatabase nosql;

    nosql.insert(
        1,
        {
            {"name", "Vishal"},
            {"email", "vishal@gmail.com"},
            {"age", "21"}
        }
    );

    nosql.insert(
        2,
        {
            {"name", "Rahul"},
            {"phone", "9876543210"},
            {"city", "Delhi"}
        }
    );

    nosql.showData();


    return 0;
}