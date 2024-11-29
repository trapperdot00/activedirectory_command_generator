#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <string>

std::string trim_left(const std::string &);
std::string trim_right(const std::string &);
std::string trim(const std::string &);

bool only_whitespace(const std::string &);
std::string::size_type find_nth(const std::string &, std::string::size_type, char);
bool commented_out(const std::string &);

#endif
