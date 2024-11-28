#include "NewUserGen.h"

#include <cstring>
#include <iostream>

void print_usage(int, char *[]);
void program(int argc, char *argv[]) {
	Flags flags(argc, argv);
	if (!flags) {
		print_usage(argc, argv);
		return;
	}

	std::ifstream csvfile(argv[1]);
	if (!csvfile.is_open())
		throw std::runtime_error(std::string(argv[1]) + " failed to open");
	
	std::ifstream mapfile(argv[2]);
	if (!mapfile.is_open())
		throw std::runtime_error(std::string(argv[2]) + " failed to open");
		
	NewUserGen usergen(csvfile, mapfile, flags);
	csvfile.close();
	mapfile.close();

	std::ofstream outfile(argv[3]);
	if (!outfile.is_open())
		throw std::runtime_error(std::string(argv[3]) + " failed to open");

	outfile << usergen;
	outfile.close();
}

void print_usage(int argc, char *argv[]) {
	std::cout << "usage: " << argv[0] << " csvfile mapfile outfile [-p] [-o] [-f]\n"
		"options:\n"
		"  [-r | --randomized-passwords]:\tflag to use random passwords for each user\n"
		"  [-o | --override]:\t\t\tflag to use overrides in mapfile for each command\n"
		"  [-f | --fallback]:\t\t\tflag to use fallbacks in mapfile for each command"
	<< std::endl;
}

int main(int argc, char *argv[]) {
	try {
		program(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
