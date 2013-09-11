#include <iostream>
#include <sstream>
#include "DCPU.h"

int main(int argc, char **argv)
{
	for(int i = 0; i < 5; i++) {
		std::cout << "Enter something: ";
		int x;
		std::cin >> x;
		std::cout << "You entered " << x << "." << std::endl;
	}
	return 0;
}

int _main(int argc, char **argv)
{
	DCPU computer;
	// Load args into RAM
	std::cout << "Loading " << argc << " instructions into RAM...";
	std::stringstream ss;
	for(int i = 0; i < argc; ++i)
	{
		ss << argv[i];
		ss >> computer.RAM[i];
	}
	std::cout << "Done." << std::endl;
	// Execute
	unsigned int num;
	do
	{
		std::cout << "DCPU state:" << std::endl;
		computer.printState();
		std::cout << "Run how many steps? (0 to exit): ";
		std::cin >> num;
		computer.run(num);
	} while(num);
	return 0;
}
