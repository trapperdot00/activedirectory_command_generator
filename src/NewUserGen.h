#ifndef NEWUSERGEN_H
#define NEWUSERGEN_H

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>

#include "CsvRow.h"
#include "MapRow.h"
#include "Flags.h"

class NewUserGen;
std::ostream &operator<<(std::ostream &, const NewUserGen &);

class NewUserGen {
public:	
	NewUserGen(std::ifstream &, std::ifstream &, Flags = Flags());

	std::ostream &print(std::ostream &) const;

private:
	std::map<std::string, std::vector<std::size_t>> get_mapper_rows() const;
//	std::map<std::string, std::vector<std::size_t>> get_command_rows() const;

	std::vector<std::map<std::string, std::string>>
		build_mapper_commands(const std::map<std::string, std::vector<std::size_t>> &) const;

	std::unordered_map<std::string, std::pair<std::string, std::vector<std::size_t>>> command_lines() const;
	void insert_commands(const std::unordered_map<std::string, std::pair<std::string, std::vector<std::size_t>>> &,
			std::vector<std::map<std::string, std::string>> &) const;
	bool has_all_mandatory_commands(const std::map<std::string, std::string> &) const;

	Flags flags;
	std::vector<CsvRow> csvrows;
	std::vector<MapRow> maprows;
};

std::string::size_type get_end_of_command(const std::string &);
std::string command_part(const std::string &);
std::string value_part(const std::string &);


#endif
