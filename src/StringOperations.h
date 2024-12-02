#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <vector>
#include <string>
#include <utility>
#include <stdexcept>
#include <sstream>

std::string trim_left(const std::string &);
std::string trim_right(const std::string &);
std::string trim(const std::string &);

std::string truncate_until(const std::string &, std::size_t);

std::vector<std::string> split(const std::string &, char);

bool only_whitespace(const std::string &);
std::string::size_type find_nth(const std::string &, std::string::size_type, char);
bool commented_out(const std::string &);

#endif
