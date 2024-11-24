#include "User.h"

User::User(std::shared_ptr<CsvHeader> csvp, const std::string &line) {
	std::istringstream stream(line);
	std::string field;
	std::size_t cnt = 0;
	while (std::getline(stream, field, csvp->delimiter())) {
		values.insert({csvp->fields[cnt++], field});
	}
}

std::ostream &operator<<(std::ostream &os, const User &u) {
	std::map<std::shared_ptr<std::string>, std::string>::const_iterator it = u.values.cbegin(),
		end = u.values.cend();
	while (it != end) {
		os << *it->first << ":\t" << it->second << '\n';
		++it;
	}
	return os;
}
