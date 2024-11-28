#ifndef CSVROW_H
#define CSVROW_H

#include "Row.h"

class CsvRow : public Row {
public:
	CsvRow(const std::string &s) : Row(s) {}
};

#endif
