#include <iostream>

using namespace std;
//A subclass should be replaceable with its parent class without changing the correctness of the program.
// A Postcondition must be statisfied after a method is executed.
// Sub classes can strengthen the Postcondition but cannot weaken it.

//Postconditions (after the function): The child should promise at least as much as the parent, and it may promise more (stronger or equal).
class Car {
protected:
//int speed; // Speed of the car 
    int speed;    

public:
    Car() {
        //speed = 0; // Initial speed of the car
        speed = 0;
    }
    
    void accelerate() {
        cout << "Accelerating" << endl;
        speed += 20;
    }

    //PostCondition : Speed must reduce after brake
    virtual void brake() {
        cout << "Applying brakes" << endl;
        speed -= 20;
    }
};

// Subclass can strengthen postcondition - Does not violate LSP
class HybridCar : public Car {
private:
    int charge;

public:

    HybridCar() : Car() {
        charge = 0;
    }

    // PostCondition : Speed must reduce after brake
    // PostCondition : Charge must increase.
    void brake() {
        cout << "Applying brakes" << endl;
        speed -= 20;
        charge += 10;
    }
};


int main() {
    Car* hybridCar = new HybridCar();
    hybridCar->brake();  // Works fine: HybridCar reduces speed and also increases charge.

    //Client feels no difference in substituting Hybrid car in place of Car.

    return 0;
}