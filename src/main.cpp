#include "NewUserGen.h"

#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

void print_usage(int argc, char *argv[]) {
	std::cout << "usage: " << argv[0] << " csvfile mapfile outfile [-p]"
		"\n\tcsvfile: CSV file that contains a header in its first row, and values for them in the next rows"
		"\n\tmapfile: file that contains rules for mapping CSV headers to New-ADUser command parameters"
		"\n\t\tformat:\tCSVheader;command"
		"\n\t\texample:\tName;-Name"
		"\n\toutfile:\tfile to write the output into"
		"\n\t[-r|--randomized-passwords]:\tflag to use random passwords for each user"
	<< std::endl;
}

int main(int argc, char *argv[]) {
	if (argc > 3 && argc < 6) {
		Flags flags;
		if (argc == 5 && (!std::strcmp(argv[4], "-r") || !std::strcmp(argv[4], "--randomized-passwords"))) {
			flags |= Flags::random_password;
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
	} else {
		print_usage(argc, argv);
	}
	return 0;
}
