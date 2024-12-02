#ifndef NEWUSERGEN_H
#define NEWUSERGEN_H

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>
#include <utility>

#include "CsvRow.h"
#include "MapRow.h"
#include "Flags.h"
#include "Random.h"
#include "NumericOperations.h"

class NewUserGen {
public:	
	NewUserGen(std::ifstream &, std::ifstream &, Flags = Flags());

	std::ostream &print(std::ostream &) const;

private:
	// Construction helper member-functions
	std::vector<CsvRow> build_csvrows(std::ifstream &);
	std::vector<MapRow> build_maprows(std::ifstream &);
	static std::vector<std::string> get_ou();
	static std::vector<std::string> get_dc();

	std::map<std::string, std::vector<std::size_t>> get_mapper_rows() const;
//	std::map<std::string, std::vector<std::size_t>> get_command_rows() const;

	std::vector<std::map<std::string, std::string>>
	build_mapper_commands(const std::map<std::string, std::vector<std::size_t>> &) const;

	std::unordered_map <std::string, std::pair<std::string, std::vector<std::size_t>>>
	command_lines() const;
	
	void insert_commands(
	const std::unordered_map<std::string, std::pair<std::string, std::vector<std::size_t>>> &,
	std::vector<std::map<std::string, std::string>> &) const;
	
	bool has_all_mandatory_commands(const std::map<std::string, std::string> &) const;
	bool has_all_suggested_commands(const std::map<std::string, std::string> &) const;

	static std::string make_unique_username(const std::string &, std::size_t);
	static void randomize_password(std::map<std::string, std::string> &);
	std::string generate_path() const;

	// DATA MEMBERS
	Flags flags;
	std::vector<CsvRow> csvrows;
	std::vector<MapRow> maprows;

	// felhasznalok/lol/xd	->	OU=xd,OU=lol,OU=felhasznalok
	// tartomany14.local	->	DC=tartomany14,DC=local
	std::vector<std::string> organization_unit;
	std::vector<std::string> domain_controller;
};

std::ostream &operator<<(std::ostream &, const NewUserGen &);

#endif
