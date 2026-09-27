#include <iostream>

using namespace std;

class BankAccount {

private:
    int balance;

public:

    BankAccount(int initialBalance) {
        balance = initialBalance;
    }

    void deposit(int amount) {
        balance += amount;
    }

    bool withdraw(int amount) {

        // Do not allow invalid withdrawal
        if (amount > balance) {
            return false;
        }

        balance -= amount;
        return true;
    }

    int getBalance() {
        return balance;
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


    // Transfer money while keeping
    // the database state valid
    bool transfer(int amount) {

        // Step 1: Withdraw from A
        if (!accountA.withdraw(amount)) {
            return false;
        }

        // Step 2: Deposit into B
        accountB.deposit(amount);

        return true;
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

    cout << "Before Transfer:\n";
    bank.showBalances();


    cout << "\nTransferring ₹20...\n";

    if (bank.transfer(20)) {
        cout << "Transfer successful\n";
    }
    else {
        cout << "Transfer failed\n";
    }


    cout << "\nAfter Transfer:\n";
    bank.showBalances();


    return 0;
}