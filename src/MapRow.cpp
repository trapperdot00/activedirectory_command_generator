#include "MapRow.h"

MapRow::MapRow(const std::string &s) : Row(s), _type(mapping) {
	if (size() != 2 || only_whitespace(right()))
		throw std::runtime_error("invalid mapfile format");
	if (parameter_value.find(parameter()) == parameter_value.cend())
		throw std::runtime_error("invalid mapfile parameter: '" + right() + '\'');
	std::size_t special_char_count = std::count(left().cbegin(), left().cend(), '$');
	if (special_char_count == 2) {
		std::string trimmed_left = trim_left(left());
		std::string::size_type end_special_char_pos = find_nth(trimmed_left, 2, '$');
		switch (command_type(trimmed_left)) {
			case Command::overrides:
				if (trimmed_left.substr(end_special_char_pos + 1).empty()) 
					throw std::runtime_error("no value after command '" + 
							trimmed_left.substr(0, end_special_char_pos + 1) + '\'');
				_type = cmd_override;
				break;
			case Command::fallbacks:
				if (trimmed_left.substr(end_special_char_pos + 1).empty()) 
					throw std::runtime_error("no value after command '" + 
							trimmed_left.substr(0, end_special_char_pos + 1) + '\'');
				_type = cmd_fallback;
				break;
			case Command::none:
				throw std::runtime_error("unrecognizable command in mapfile: '" + trimmed_left + '\'');
				break;
			default:
				throw std::runtime_error("unhandled command: '" + trimmed_left + '\'');
				break;
		}
	} else {
		
	}
}

std::string MapRow::parameter() const {
	return trim(right());
}

std::ostream &MapRow::print(std::ostream &os) const {
	return os << left() << ":\t" << right();
}

int get_command_type(const std::string &s) {
	if (s == "$OVERRIDE$")
		return MapRow::cmd_override;
	if (s == "$FALLBACK$")
		return MapRow::cmd_fallback;
	throw std::runtime_error("get_command_type received an invalid command: '" + s + "'");
}
