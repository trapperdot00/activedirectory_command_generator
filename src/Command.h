#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>

enum class Command {
	none,
	overrides,
	fallbacks
};

Command command_type(const std::string &);

#endif
