#include <iostream>

using namespace std;

class BankAccount {

private:
    int balance;

public:

    BankAccount(int balance) {
        this->balance = balance;
    }

    bool withdraw(int amount) {

        if (amount > balance) {
            return false;
        }

        balance -= amount;
        return true;
    }

    void deposit(int amount) {
        balance += amount;
    }

    int getBalance() {
        return balance;
    }

    // Used for rollback
    void setBalance(int balance) {
        this->balance = balance;
    }
};


class Bank {

private:
    BankAccount accountA;
    BankAccount accountB;

public:

    Bank(int balanceA, int balanceB)
        : accountA(balanceA),
          accountB(balanceB) {
    }


    bool transfer(int amount) {

        // Save old state
        int oldBalanceA = accountA.getBalance();
        int oldBalanceB = accountB.getBalance();


        // Step 1: Withdraw money
        if (!accountA.withdraw(amount)) {

            cout << "Transaction failed!\n";
            return false;
        }


        // Simulate a failure
        bool depositSuccessful = false;


        // Step 2: Deposit money
        if (depositSuccessful) {

            accountB.deposit(amount);

            cout << "Transaction committed!\n";
            return true;
        }


        // -------------------------
        // Rollback
        // -------------------------

        accountA.setBalance(oldBalanceA);
        accountB.setBalance(oldBalanceB);

        cout << "Transaction failed!\n";
        cout << "Rollback performed!\n";

        return false;
    }


    void showBalances() {

        cout << "Account A: ₹"
             << accountA.getBalance()
             << endl;

        cout << "Account B: ₹"
             << accountB.getBalance()
             << endl;
    }
};


int main() {

    Bank bank(100, 50);


    cout << "Before Transaction:\n";
    bank.showBalances();


    cout << "\nTransferring ₹20...\n";

    bank.transfer(20);


    cout << "\nAfter Transaction:\n";
    bank.showBalances();


    return 0;
}