#ifndef ROW_H
#define ROW_H

#include <vector>
#include <string>
#include <sstream>
#include <cstddef>
#include <iostream>
#include <stdexcept>

class Row {
public:
	explicit Row(const std::string &);
	virtual ~Row() = default;

	std::string &operator[](std::size_t n) { return fields[n]; }
	const std::string &operator[](std::size_t n) const { return fields[n]; }

	std::vector<std::string>::size_type size() const { return fields.size(); }
	bool empty() const { return !size(); }

	virtual std::ostream &print(std::ostream &) const;
private:
	std::vector<std::string> fields;
};

std::ostream &operator<<(std::ostream &, const Row &);

#endif
