#include <iostream>
using namespace std;
//"This code implements the Chain of Responsibility pattern. Each denomination (₹1000, ₹500, ₹200, ₹100) is represented by a separate handler. When a withdrawal request arrives, each handler dispenses as many notes of its denomination as possible and forwards the remaining amount to the next handler. The client only interacts with the first handler, making the system loosely coupled, easy to extend with new denominations, and compliant with the Open/Closed Principle."
// Abstract Handler (Base Class)
//Chain of Responsibility is a Behavioral Design Pattern in which a request is passed through a chain of handlers until one or more handlers process it.
class MoneyHandler {
protected:
    MoneyHandler *nextHandler;

public:
    MoneyHandler() {
        this->nextHandler = nullptr;
    }

    void setNextHandler(MoneyHandler *next) { 
        nextHandler = next; 
    }

    virtual void dispense(int amount) = 0;
};
/*OVERALL ARCHITECTURE
          Client
             |
             |
             V
     ThousandHandler
             |
             V
     FiveHundredHandler
             |
             V
     TwoHundredHandler
             |
             V
      HundredHandler
             |
             V
          NullHandler
*/
class ThousandHandler : public MoneyHandler {
private:
    int numNotes;

public:
    ThousandHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int notesNeeded = amount / 1000;

        if(notesNeeded > numNotes) {
            notesNeeded = numNotes;
            numNotes = 0;
        }
        else {
            numNotes -= notesNeeded;
        }

        if(notesNeeded > 0)
            cout << "Dispensing " << notesNeeded << " x ₹1000 notes.\n";

        int remainingAmount = amount - (notesNeeded * 1000);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainingAmount);
            else {
                cout << "Remaining amount of " << remainingAmount << " cannot be fulfilled (Insufficinet fund in ATM)\n";
            }
        }
    }
};

// Concrete Handler for 500 Rs Notes
class FiveHundredHandler : public MoneyHandler {
private:
    int numNotes;

public:
    FiveHundredHandler(int numNotes) {
        this->numNotes = numNotes;    
    }

    void dispense(int amount) override {
        int notesNeeded = amount / 500;

        if(notesNeeded > numNotes) {
            notesNeeded = numNotes;
            numNotes = 0;
        }
        else {
            numNotes -= notesNeeded;
        }

        if(notesNeeded > 0)
            cout << "Dispensing " << notesNeeded << " x ₹500 notes.\n";

        int remainingAmount = amount - (notesNeeded * 500);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainingAmount);
            else {
                cout << "Remaining amount of " << remainingAmount << " cannot be fulfilled (Insufficinet fund in ATM)\n";
            }
        }
    }
};

// Concrete Handler for 200 Rs Notes
class TwoHundredHandler : public MoneyHandler {
private:
    int numNotes;

public:
    TwoHundredHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int notesNeeded = amount / 200;

        if(notesNeeded > numNotes) {
            notesNeeded = numNotes;
            numNotes = 0;
        }
        else {
            numNotes -= notesNeeded;
        }

        if(notesNeeded > 0)
            cout << "Dispensing " << notesNeeded << " x ₹200 notes.\n";

        int remainingAmount = amount - (notesNeeded * 200);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainingAmount);
            else {
                cout << "Remaining amount of " << remainingAmount << " cannot be fulfilled (Insufficinet fund in ATM)\n";
            }
        }
    }
};

// Concrete Handler for 100 Rs Notes
class HundredHandler : public MoneyHandler {
private:
    int numNotes;

public:
    HundredHandler(int numNotes) {
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        int notesNeeded = amount / 100;

        if(notesNeeded > numNotes) {
            notesNeeded = numNotes;
            numNotes = 0;
        }
        else {
            numNotes -= notesNeeded;
        }

        if(notesNeeded > 0)
            cout << "Dispensing " << notesNeeded << " x ₹100 notes.\n";

        int remainingAmount = amount - (notesNeeded * 100);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr) nextHandler->dispense(remainingAmount);
            else {
                cout << "Remaining amount of " << remainingAmount << " cannot be fulfilled (Insufficinet fund in ATM)\n";
            }
        }
    }
};

// Client Code
int main() {
    // Creating handlers for each note type
    MoneyHandler* thousandHandler = new ThousandHandler(3);
    MoneyHandler* fiveHundredHandler = new FiveHundredHandler(5);
    MoneyHandler* twoHundredHandler= new TwoHundredHandler(10);
    MoneyHandler* hundredHandler= new HundredHandler(20);

    // Setting up the chain of responsibility
    thousandHandler->setNextHandler(fiveHundredHandler);
    fiveHundredHandler->setNextHandler(twoHundredHandler);
    twoHundredHandler->setNextHandler(hundredHandler);

    int amountToWithdraw = 2000;

    // Initiating the chain
    cout << "\nDispensing amount: ₹" << amountToWithdraw << endl;
    thousandHandler->dispense(amountToWithdraw);

    return 0;
}

/*
Q5. Which SOLID principles are followed?
->Single Responsibility Principle (SRP): Each handler manages one denomination.
->Open/Closed Principle (OCP): New denominations can be added by creating new handlers.
->Liskov Substitution Principle (LSP): Every concrete handler can be used wherever a MoneyHandler is expected.
*/