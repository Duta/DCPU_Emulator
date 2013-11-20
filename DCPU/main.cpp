#include <iostream>
#include <fstream>
#include "DCPU.h"
#include "LEM1802.h"
#include "HMD2043.h"
#include "HMU1440.h"

int main(int argc, char *argv[])
{
    using std::ios;
    // Check that the correct number of arguments were given
    if(argc < 2)
    {
        std::cout << "ERROR: Insufficient arguments provided." << std::endl;
        std::cout << "USAGE:" << std::endl;
        std::cout << "    DCPU code_file [-printStart] [-printEnd]" << std::endl;
        return 1;
    }
    // "Create" the computer (if only it were that easy)
    DCPU computer;
    // Load the program
    unsigned int numInstructions = 0;
    std::ifstream file(argv[1], ios::in | ios::binary | ios::ate);
    if(file.is_open())
    {
        std::ifstream::pos_type size = file.tellg();
        char *tempArray = new char[(unsigned int)size];
        file.seekg(0, ios::beg);
        file.read(tempArray, size);
        file.close();
        for(int i = 0; i < RAM_SIZE && i < size/2; i++)
        {
            char upper = tempArray[i*2];
            char lower = tempArray[i*2 + 1];
            computer.RAM[i] = (upper << 8) | lower;
            numInstructions = i + 1;
        }
        delete[] tempArray;
    }
    else
    {
        std::cout << "ERROR: Unable to read code file." << std::endl;
        return 1;
    }
    // Connect hardware
    LEM1802 lem;
    HMD2043 hmd;
    HMU1440 hmu;
    hmd.loadDisk(&hmu);
#define CONNECT_HARDWARE(x) std::cout << "Connecting " << x.toString() << std::endl; \
                            computer.connectHardware(&x)
    CONNECT_HARDWARE(lem);
    CONNECT_HARDWARE(hmd);
#undef CONNECT_HARDWARE
    // Print initial state
    std::string printStart("-printStart");
    if((argc >= 3 && printStart == argv[2])
    || (argc >= 4 && printStart == argv[3])) {
        std::cout << "Initial DCPU state:" << std::endl;
        computer.printState();
    }
    // Execute
    std::cout << "Running " << std::dec << numInstructions << " instructions...";
    computer.run(numInstructions);
    std::cout << "Done." << std::endl;
    // Print final state
    std::string printEnd("-printEnd");
    if((argc >= 3 && printEnd == argv[2])
    || (argc >= 4 && printEnd == argv[3])) {
        std::cout << "Final DCPU state:" << std::endl;
        computer.printState();
    }
    // Disconnect hardware
#define DISCONNECT_HARDWARE(x) std::cout << "Disconnecting " << x.toString() << std::endl; \
                               computer.disconnectHardware(&x)
    DISCONNECT_HARDWARE(lem);
    DISCONNECT_HARDWARE(hmd);
#undef DISCONNECT_HARDWARE
    return 0;
}
