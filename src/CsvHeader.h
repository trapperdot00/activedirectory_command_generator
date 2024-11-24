#ifndef CSVHEADER_H
#define CSVHEADER_H

#include <vector>
#include <string>
#include <sstream>
#include <cstddef>
#include <memory>
#include <fstream>
#include <stdexcept>

class User;
class NewUserGen;

class CsvHeader {
	friend std::ostream &operator<<(std::ostream &, const CsvHeader &);
	friend class User;
	friend class NewUserGen;

	explicit CsvHeader(const std::string &, char = ';');
	explicit CsvHeader(std::ifstream &);

	std::string operator[](std::size_t n) const { return *(fields[n]); }
	char delimiter() const { return delim; }

	std::vector<std::shared_ptr<std::string>> fields;
	char delim = ';';
};

#endif
