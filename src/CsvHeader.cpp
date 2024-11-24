#include "CsvHeader.h"

CsvHeader::CsvHeader(const std::string &line, char d) : delim(d) {
	std::istringstream stream(line);
	std::string field;
	while (std::getline(stream, field, delim))
		fields.push_back(std::make_shared<std::string>(field));
}

CsvHeader::CsvHeader(std::ifstream &in_file) {
	std::string line;
	if (!std::getline(in_file, line))
		throw std::runtime_error("no input for csvheader");
	std::istringstream stream(line);
	std::string field;
	while (std::getline(stream, field, delim))
		fields.push_back(std::make_shared<std::string>(field));
}

std::ostream &operator<<(std::ostream &os, const CsvHeader &u) {
	std::vector<std::string>::size_type sz = u.fields.size();
	for (std::vector<std::string>::size_type i = 0; i != sz; ++i) {
		os << i << '\t' << u.fields[i];
		if (i < sz - 1)
			os << '\n';
	}
	return os;
}
