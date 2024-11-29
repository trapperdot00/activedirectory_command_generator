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

bool only_whitespace(const std::string &s) {
	return s.find_first_not_of("\n\t ") == std::string::npos;
}

std::string::size_type find_nth(const std::string &s, std::string::size_type n, char c) {
	if (!n) return std::string::npos;
	std::string::size_type pos = 0, count = 0;
	while (pos != s.size()) {
		if (s[pos] == c)
			++count;
		if (count == n)
			return pos;
		++pos;
	}
	return std::string::npos;
}

bool commented_out(const std::string &s) {
	static constexpr char comment = '#';
	std::string::size_type c_pos = s.find(comment);
	if (c_pos == std::string::npos)
		return false;
	std::string::size_type v_pos = s.find_first_not_of("\n\t ");
	if (c_pos == v_pos)
		return true;
	return false;
}
