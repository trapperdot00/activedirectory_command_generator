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

#include "User.h"
#include "Command.h"

class NewUserGen;
std::ostream &operator<<(std::ostream &, const NewUserGen &);

class NewUserGen {
public:
	explicit NewUserGen(std::ifstream &, const std::map<std::string, std::string> &);

	std::vector<std::string> operator()() const;

	std::ostream &print(std::ostream &) const;
private:
	static std::map<std::string, Command> validate_map(const std::map<std::string, std::string> &);
	static std::string format_arg(const Command &, const std::string &);

	std::shared_ptr<CsvHeader> header;
	std::vector<User> users;
	std::map<std::string, Command> header_to_cmd;
	static const std::set<Command> commands;
};

#endif
