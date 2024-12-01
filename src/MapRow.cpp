#include "MapRow.h"

MapRow::MapRow(const std::string &s) : Row(s), _type(mapping) {
	if (size() != 2 || only_whitespace(right()))
		throw std::runtime_error("invalid mapfile format");
	if (parameter_value.find(parameter()) == parameter_value.cend())
		throw std::runtime_error("invalid mapfile parameter: '" + right() + '\'');
	if (is_command(left())) {
		const std::string &t_left = trim_left(left());
		const std::string &cmd = command_part(t_left);
		const std::string &value = value_part(t_left);
		if (value.empty())
			throw std::runtime_error("no value after command '" + cmd + "' mapped to '" + parameter() + '\'');
		_type = get_command_type(cmd);
	}
}

const std::map<std::string, int> MapRow::cmd_name = {
	{"$OVERRIDE$", cmd_override},
	{"$FALLBACK$", cmd_fallback}
};

std::string MapRow::parameter() const {
	return trim(right());
}

std::ostream &MapRow::print(std::ostream &os) const {
	return os << left() << ":\t" << right();
}

std::string::size_type get_end_of_command(const std::string &s, const std::string &callerfuncname) {
	std::string::size_type ret = find_nth(s, 2, '$');
	if (ret == std::string::npos)
		throw std::runtime_error(callerfuncname + " received a non-command string");
	return ret;
}

std::string command_part(const std::string &s) {
	std::string::size_type end_pos = get_end_of_command(s, "command_part");
	std::string ret = s.substr(0, end_pos + 1);
	return ret;
}

std::string value_part(const std::string &s) {
	std::string::size_type end_pos = get_end_of_command(s, "value_part");
	if (end_pos + 1 >= s.size())
		return "";
	std::string ret = s.substr(end_pos + 1);
	return ret;
}

bool is_command(const std::string &s) {
	for (std::size_t i = 1; i != 3; ++i)
		if (find_nth(s, i, '$') == std::string::npos)
			return false;
	return true;
}

int get_command_type(const std::string &s) {
	const std::map<std::string, int>::const_iterator it = MapRow::cmd_name.find(s);
	if (it != MapRow::cmd_name.cend())
		return it->second;
	throw std::runtime_error("get_command_type received an invalid command: '" + s + "'");
}
