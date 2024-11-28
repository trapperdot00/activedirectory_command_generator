#ifndef NEWUSERGEN_H
#define NEWUSERGEN_H

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

#include "CsvRow.h"
#include "MapRow.h"
#include "Flags.h"

class NewUserGen;
std::ostream &operator<<(std::ostream &, const NewUserGen &);

class NewUserGen {
public:	
	NewUserGen(std::ifstream &, std::ifstream &, Flags = Flags());

	std::ostream &print(std::ostream &) const;

private:
	Flags flags;
	std::vector<CsvRow> csvrows;
	std::vector<MapRow> maprows;
};

#endif
