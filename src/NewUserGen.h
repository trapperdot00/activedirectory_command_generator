#ifndef NEWUSERGEN_H
#define NEWUSERGEN_H

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <iostream>

#include "StringOperations.h"
#include "Command.h"
#include "Row.h"
#include "Flags.h"
#include "Random.h"

class NewUserGen;
std::ostream &operator<<(std::ostream &, const NewUserGen &);

class NewUserGen {
public:	
	NewUserGen(std::ifstream &, std::ifstream &, Flags = Flags());

	std::ostream &print(std::ostream &) const;
private:
	static std::map<std::string, std::string> parse_mapfile(std::ifstream &);
	void read_rows(std::istream &);
	void validate_map(const std::map<std::string, std::string> &);
	static std::string format_arg(const Command &, const std::string &);
	void build_positions();
	static bool is_special(const std::string &);

	static const std::set<Command> commands;

	Flags flags;
	std::map<std::string, Command> header_to_cmd;
	std::vector<Row> rows;

	std::vector<std::size_t> valid_header_pos;
};

#endif
