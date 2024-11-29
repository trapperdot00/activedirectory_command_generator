#ifndef CSVROW_H
#define CSVROW_H

#include "Row.h"

class CsvRow : public Row {
public:
	CsvRow(const std::string &s) : Row(s) {}

	std::vector<std::string>::iterator begin() { return fields.begin(); }
	std::vector<std::string>::iterator end() { return fields.end(); }
	std::vector<std::string>::const_iterator cbegin() const { return fields.cbegin(); }
	std::vector<std::string>::const_iterator cend() const { return fields.cend(); }
	std::vector<std::string>::const_iterator begin() const { return cbegin(); }
	std::vector<std::string>::const_iterator end() const { return cend(); }
};

#endif
