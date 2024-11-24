#ifndef USER_H
#define USER_H

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <memory>

#include "CsvHeader.h"

class NewUserGen;

class User {
	friend class NewUserGen;
	friend std::ostream &operator<<(std::ostream &, const User &);
	User(std::shared_ptr<CsvHeader>, const std::string &);

	std::map<std::shared_ptr<std::string>, std::string> values;
};

#endif
