#include "Command.h"

Command command_type(const std::string &s) {
	static std::map<std::string, Command> commands =
		{ {"$OVERRIDE$", Command::overrides}, {"$FALLBACK$", Command::fallbacks} };
	for (const std::pair<std::string, Command> &p : commands) {
		if (s.find(p.first) != std::string::npos)
			return p.second;
	}
	return Command::none;
}
