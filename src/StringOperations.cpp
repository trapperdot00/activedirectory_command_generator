#include "StringOperations.h"

std::string trim_left(const std::string &s) {
	std::string::size_type pos = s.find_first_not_of("\n\t ");
	if (pos == std::string::npos)
		return "";
	return s.substr(pos);
}

std::string trim_right(const std::string &s) {
	std::string::size_type pos = s.find_last_not_of("\n\t ");
	if (pos == std::string::npos)
		return "";
	return s.substr(0, pos + 1);
}

std::string trim(const std::string &s) {
	return trim_right(trim_left(s));
}
