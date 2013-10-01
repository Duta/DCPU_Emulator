#include <iostream>
#include <sstream>
#include "DCPU.h"
#include "LEM1802.h"
#include "HMD2043.h"
#include "HMU1440.h"

int main(int argc, char *argv[])
{
	DCPU computer;
	// Load args into RAM
	std::cout << "Loading " << std::dec << argc - 1 << " instructions into RAM...";
	std::stringstream ss;
	for(int i = 1; i < argc && i <= RAM_SIZE; ++i)
	{
		ss << argv[i];
		ss >> computer.RAM[i - 1];
		ss.clear();
	}
	std::cout << "Done." << std::endl;
	// Connect hardware
	LEM1802 lem;
	HMD2043 hmd;
	HMU1440 hmu;
	hmd.loadDisk(&hmu);
#define CONNECT_HARDWARE(x) std::cout << "Connecting " << x.toString() << std::endl; \
                            computer.connectHardware(&x);
	CONNECT_HARDWARE(lem)
	CONNECT_HARDWARE(hmd)
#undef CONNECT_HARDWARE
	// Print initial state
	std::cout << "DCPU state:" << std::endl;
	computer.printState();
	// Execute
	std::cout << "Running " << std::dec << argc - 1 << " instructions...";
	computer.run(argc - 1);
	std::cout << "Done." << std::endl;
	// Print final state
	std::cout << "DCPU state:" << std::endl;
	computer.printState();
	// Disconnect hardware
#define DISCONNECT_HARDWARE(x) std::cout << "Disconnecting " << x.toString() << std::endl; \
                               computer.disconnectHardware(&x);
	DISCONNECT_HARDWARE(lem)
	DISCONNECT_HARDWARE(hmd)
#undef DISCONNECT_HARDWARE
	return 0;
}
