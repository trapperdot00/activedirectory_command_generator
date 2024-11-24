#include "NewUserGen.h"

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <cstddef>

std::map<std::string, std::string> parse_mapfile(std::ifstream &file) {
	std::map<std::string, std::string> ret;
	std::set<std::string> existing_commands;
	std::string line;
	std::size_t lines_read = 0;
	while (std::getline(file, line)) {
		if (line.empty()) continue;
		std::istringstream stream(line);
		std::string header, command;
		if (!std::getline(stream, header, ';') || !std::getline(stream, command))
			throw std::runtime_error("mapfile invalid format");
		if (existing_commands.find(command) != existing_commands.cend())
			throw std::runtime_error("multiple bindings to the same command in mapfile");
		existing_commands.insert(command);
		ret.insert({header, command});
		++lines_read;
	}
	if (lines_read == 0)
		throw std::runtime_error("mapfile empty");
	return ret;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		std::cout << "usage: " << argv[0] << " csvfile mapfile outfile"
			"\n\tcsvfile: CSV file that contains a header in its first row, and values for them in the next rows"
			"\n\tmapfile: file that contains rules for mapping CSV headers to New-ADUser command parameters"
			"\n\t\tformat:\tCSVheader;command"
			"\n\t\texample:\tName;-Name"
			"\n\toutfile: file to write the output into"
		<< std::endl;
	} else {
		std::ifstream csvfile(argv[1]);
		if (!csvfile.is_open())
			throw std::runtime_error(std::string(argv[1]) + " failed to open");
		
		std::ifstream mapfile(argv[2]);
		if (!mapfile.is_open())
			throw std::runtime_error(std::string(argv[2]) + " failed to open");

		std::map<std::string, std::string> mappings = parse_mapfile(mapfile);
		mapfile.close();

		NewUserGen usergen(csvfile, mappings);
		csvfile.close();

		std::vector<std::string> cmds = usergen();

		std::ofstream outfile(argv[3]);
		if (!outfile.is_open())
			throw std::runtime_error(std::string(argv[3]) + " failed to open");

		for (const std::string &cmd : cmds)
			outfile << cmd << std::endl;

	}
	return 0;
}
