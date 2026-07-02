#include <iostream>

using namespace std;

// A Precondition must be statisfied before a method can be executed.
// Sub classes can weaken the precondition but cannot strengthen it.
//Preconditions (before the function): The child should ask for less from the caller (weaker or equal).
class User {
public:
    // Precondition: Password must be at least 8 characters long
    virtual void setPassword(string password) {
        if (password.length() < 8) {
            throw invalid_argument("Password must be at least 8 characters long!");
        }
        cout << "Password set successfully" << endl;
    }
};
/*
| Contract      | Parent Says     | Child Can Do                      | Allowed?         |
| ------------- | --------------- | --------------------------------- | ---------------- |
| Precondition  | Password ≥ 8    | Password ≥ 6                      | ✅ Yes (weaker)   |
| Precondition  | Password ≥ 8    | Password ≥ 12                     | ❌ No (stronger)  |
| Postcondition | Speed decreases | Speed decreases + Battery charges | ✅ Yes (stronger) |
| Postcondition | Speed decreases | Doesn't reduce speed              | ❌ No (weaker)    |

*/
class AdminUser : public User {
public:
    // Precondition: Password must be at least 6 characters
    void setPassword(string password) override {
        if (password.length() < 6) { 
            throw invalid_argument("Password must be at least 6 characters long!");
        }
        cout << "Password set successfully" << endl;
    }
};

int main() {
    User* user = new AdminUser();
    user->setPassword("Admin1");  // Works fine: AdminUser allows shorter passwords

    return 0;
}