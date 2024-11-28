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

	std::ostream &print(std::ostream &) const override;

	static const std::map<std::string, ValueType> parameter_value;
	enum {
		mapping,
		cmd_override,
		cmd_fallback
	};
	int type() const { return _type; }
private:
	int _type;
};

#endif
