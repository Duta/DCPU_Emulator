#include <iostream>
#include <fstream>
#include "DCPU.h"
#include "LEM1802.h"
#include "HMD2043.h"
#include "HMU1440.h"

bool hasParam(std::string param, int argc, char *argv[])
{
    for(int i = 2; i < argc; ++i)
    {
        if(param == argv[i])
        {
            return true;
        }
    }
    return false;
}

void printUsage()
{
    std::cout << "USAGE:" << std::endl;
    std::cout << "    DCPU code_file [-printStart] [-printEnd] [-printSteps]" << std::endl;
}

void printError(std::string error)
{
    std::cout << "ERROR: " << error << std::endl;
}

int main(int argc, char *argv[])
{
    using std::ios;
    // Check that the correct number of arguments were given
    if(argc < 2)
    {
        printError("Insufficient arguments provided.");
        printUsage();
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
        printError("Unable to read code file.");
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
    if(hasParam("-printStart", argc, argv)) {
        std::cout << "Initial DCPU state:" << std::endl;
        computer.printState();
    }
    // Execute
    std::cout << "Running " << std::dec << numInstructions << " instructions...";
    computer.run(numInstructions, hasParam("-printSteps", argc, argv));
    std::cout << "Done." << std::endl;
    // Print final state
    if(hasParam("-printEnd", argc, argv)) {
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
