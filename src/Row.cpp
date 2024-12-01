#include "Row.h"

Row::Row(const std::string &line) {
	std::string field;
	std::istringstream stream(line);

	// Read fields until the next delimiter
	while (std::getline(stream,field, ';'))
		fields.push_back(field);
}

std::ostream &Row::print(std::ostream &os) const {
	for (std::vector<std::string>::size_type i = 0; i != size(); ++i) {
		if (i)
			os << '\n';
		os << this->operator[](i);
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const Row &r) {
	return r.print(os);
}
