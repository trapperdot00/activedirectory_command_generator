#ifndef MAPROW_H
#define MAPROW_H

#include "Row.h"
#include "StringOperations.h"
#include "ValueType.h"
#include "Command.h"

#include <algorithm>
#include <map>

class MapRow : public Row {
public:
	MapRow(const std::string &);

	std::string &left() { return Row::operator[](0); }
	std::string &right() { return Row::operator[](1); }
	const std::string &left() const { return Row::operator[](0); }
	const std::string &right() const { return Row::operator[](1); }

	std::string row() const { return left() + ';' + right(); }

	std::ostream &print(std::ostream &) const override;

	enum {
		mapping,
		cmd_override,
		cmd_fallback
	};
	static const std::map<std::string, int> cmd_name;
	int type() const { return _type; }
	std::string parameter() const;
private:
	int _type;
};

std::string::size_type get_end_of_command(const std::string &, const std::string &);
std::string command_part(const std::string &);
std::string value_part(const std::string &);

bool is_command(const std::string &);
int get_command_type(const std::string &);

#endif
