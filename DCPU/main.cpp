#include <iostream>
#include <sstream>
#include "DCPU.h"

int main(int argc, char **argv)
{
	DCPU computer;
	// Load args into RAM
	std::cout << "Loading " << argc - 1 << " instructions into RAM...";
	std::stringstream ss;
	for(int i = 1; i < argc; ++i)
	{
		ss << argv[i];
		ss >> computer.RAM[i - 1];
		ss.clear();
	}
	std::cout << "Done." << std::endl;
	// Print initial state
	std::cout << "DCPU state:" << std::endl;
	computer.printState();
	// Execute
	std::cout << "Running " << argc - 1 << " instructions...";
	computer.run(argc - 1);
	std::cout << "Done." << std::endl;
	// Print final state
	std::cout << "DCPU state:" << std::endl;
	computer.printState();
	return 0;
}
