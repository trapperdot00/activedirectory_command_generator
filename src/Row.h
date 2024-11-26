#ifndef ROW_H
#define ROW_H

#include <vector>
#include <string>
#include <sstream>
#include <cstddef>
#include <iostream>

class Row {
public:
	explicit Row(const std::string &);

	std::string &operator[](std::size_t n) { return fields[n]; }
	const std::string &operator[](std::size_t n) const { return fields[n]; }

	std::vector<std::string>::size_type size() const { return fields.size(); }
private:
	std::vector<std::string> fields;
};

std::ostream &operator<<(std::ostream &, const Row &);

#endif
