#include <iostream>
using namespace std;

/*
                Client
                  |
                  |
                  V
          ComputerFacade
                  |
   -------------------------------------
   |      |      |      |      |      |
   V      V      V      V      V      V
 Power  Cooling  BIOS  HDD    CPU   Memory
                   |
               OperatingSystem
*/
// Subsystems
class PowerSupply {
public:
    void providePower() {
        cout << "Power Supply: Providing power..." << endl;
    }
};

class CoolingSystem {
public:
    void startFans() {
        cout << "Cooling System: Fans started..." << endl;
    }
};

class CPU {
public:
    void initialize() {
        cout << "CPU: Initialization started..." << endl;
    }
};

class Memory {
public:
    void selfTest() {
        cout << "Memory: Self-test passed..." << endl;
    }
};

class HardDrive {
public:
    void spinUp() {
        cout << "Hard Drive: Spinning up..." << endl;
    }
};

class BIOS {
public:
    void boot(CPU& cpu, Memory& memory) {
        cout << "BIOS: Booting CPU and Memory checks..." << endl;
        cpu.initialize();
        memory.selfTest();
    }
};

class OperatingSystem {
public:
    void load() {
        cout << "Operating System: Loading into memory..." << endl;
    }
};

// Facade
class ComputerFacade {
private:
    PowerSupply powerSupply;
    CoolingSystem coolingSystem;
    CPU cpu;
    Memory memory;
    HardDrive hardDrive;
    BIOS bios;
    OperatingSystem os;

public:
    void startComputer() {
        cout << "----- Starting Computer -----" << endl;
        powerSupply.providePower();
        coolingSystem.startFans();
        bios.boot(cpu, memory);
        hardDrive.spinUp();
        os.load();
        cout << "Computer Booted Successfully!" << endl;
    }
};

// Client
int main() {
    ComputerFacade computer;
    computer.startComputer();

    return 0;
}

//Facade Design Pattern provides a single simplified interface to a set of complex subsystem classes. Instead of interacting with multiple classes directly, the client communicates with one Facade object, which coordinates all subsystem operations internally. In this example, ComputerFacade hides the entire computer boot process behind a single startComputer() method.