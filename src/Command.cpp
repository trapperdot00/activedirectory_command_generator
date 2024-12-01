#include "Command.h"

Command command_type(const std::string &s) {
	// Associate strings with the enumerators
	static const std::map<std::string, Command> commands =
		{ {"$OVERRIDE$", Command::overrides}, {"$FALLBACK$", Command::fallbacks} };

	// Find the key of the given string in map, then return its value
	std::map<std::string, Command>::const_iterator it = commands.find(s);
	if (it != commands.cend())
		return it->second;
	
	// No match
	return Command::none;
}
