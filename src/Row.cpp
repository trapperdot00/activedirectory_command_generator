#include "Row.h"

Row::Row(const std::string &line) {
	std::string field;
	for (std::istringstream stream(line); std::getline(stream,field, ';'); fields.push_back(field)) ;
}

std::ostream &operator<<(std::ostream &os, const Row &r) {
	for (std::vector<std::string>::size_type i = 0; i != r.size(); ++i)
		os << r[i] << '\n';
	return os;
}
