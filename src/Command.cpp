#include "Command.h"

bool operator<(const Command &lhs, const Command &rhs) {
	return lhs.value_ < rhs.value_;
}

bool operator==(const Command &lhs, const char *rhs) {
	return std::string(lhs) == std::string(rhs);
}

bool operator!=(const Command &lhs, const char *rhs) {
	return !(lhs == rhs);
}
