#include "NewUserGen.h"

NewUserGen::NewUserGen(std::ifstream &csvfile, std::ifstream &mapfile, Flags f)
	: flags(f)
{
	for (std::string line; std::getline(csvfile, line); csvrows.emplace_back(line)) ;
	if (csvrows.empty())
		throw std::runtime_error("csvfile is empty");
	if (csvrows.size() == 1)
		throw std::runtime_error("csvfile contains only the header");

	std::set<std::string> existing_parameter_mappings;
	for (std::string line; std::getline(mapfile, line); ) {
		if (commented_out(line)) continue;
	   	maprows.emplace_back(line);;
		const MapRow &maprow = maprows[maprows.size() - 1];
		const std::string &parameter = maprow.parameter();
		switch (maprow.type()) {
		case MapRow::mapping:
			if (existing_parameter_mappings.find(parameter) != existing_parameter_mappings.cend())
				throw std::runtime_error("multiple bindings to parameter " + parameter);
			existing_parameter_mappings.insert(parameter);
			break;
		case MapRow::cmd_override:
			// TODO
			break;
		case MapRow::cmd_fallback:
			// TODO
			break;
		default:
			break;
		}
	}
	if (maprows.size() == 0)
		throw std::runtime_error("mapfile is empty or has only commented out rows");
	if (std::count_if(maprows.cbegin(), maprows.cend(), [](const MapRow &m){ return m.type() == MapRow::mapping; }) == 0)
		throw std::runtime_error("mapfile contains only command rows");
}

std::ostream &NewUserGen::print(std::ostream &os) const {
	// build map
	std::map<std::string, std::string> header_to_param;
	std::map<std::string, std::string> override_values;
	std::map<std::string, std::string> fallback_values;
	for (const MapRow &row : maprows) {
		if (row.type() == MapRow::mapping) {
			header_to_param.insert({row.right(), row.left()});
		} else if (row.type() == MapRow::cmd_override) {
			override_values.insert({row.parameter(), row.left().substr(find_nth(row.left(), 2, '$') + 1)});
		} else if (row.type() == MapRow::cmd_fallback) {
			fallback_values.insert({row.parameter(), row.left().substr(find_nth(row.left(), 2, '$') + 1)});
		}
	}

	std::vector<std::vector<std::string>> command_lines;
	for (std::size_t csvrow_i = 1; csvrow_i < csvrows.size(); ++csvrow_i) {
		std::map<std::string, std::string> commands;
		const CsvRow &header_row = csvrows[0];
		const CsvRow &curr_csvrow = csvrows[csvrow_i];
		for (std::vector<std::string>::size_type csvfield_i = 0; csvfield_i != csvrows[csvrow_i].size(); ++csvfield_i) {
			const std::string &curr_csvfield = curr_csvrow[csvfield_i];
			const std::string &curr_header_field = header_row[csvfield_i];
			for (std::size_t maprow_i = 0; maprow_i != maprows.size(); ++maprow_i) {	
				const MapRow &curr_maprow = maprows[maprow_i];
				const std::string &parameter = curr_maprow.parameter();
				const std::string &argument = curr_csvfield;
				const ValueType &type = MapRow::parameter_value.find(parameter)->second;
				std::map<std::string, std::string>::const_iterator m_it;
				if (argument.empty()) break;
				if (curr_maprow.type() == MapRow::mapping) {
					if (curr_header_field == curr_maprow.left()) {
						std::string formatted = format_argument(argument, type);
						commands.insert({parameter, formatted});
					}
				} else { 
					if ((m_it = header_to_param.find(parameter)) != header_to_param.cend() && m_it->second == curr_header_field) {
						if (curr_maprow.type() == MapRow::cmd_override) {
							if (override_values.find(parameter) != override_values.end())
								commands[parameter] = format_argument(override_values.find(parameter)->second, type);
						} else if (curr_maprow.type() == MapRow::cmd_fallback) {
							if (fallback_values.find(parameter) != fallback_values.end())
								commands.insert({parameter, format_argument(fallback_values.find(parameter)->second, type)});
						}
					}
				}
			}
		}
		
		for (const auto &p : override_values) {
			if (commands.find(p.first) == commands.cend())
					commands.insert({p.first, format_argument(p.second, MapRow::parameter_value.find(p.first)->second)});
		}
		for (const auto &p : fallback_values) {
			if (commands.find(p.first) == commands.cend())
					commands.insert({p.first, format_argument(p.second, MapRow::parameter_value.find(p.first)->second)});
		}

		std::vector<std::string> user_commands;
		for (const auto &p : commands) {
			user_commands.push_back(p.first + ' ' + p.second);
		}
		command_lines.push_back(user_commands);
	}
	for (std::vector<std::string>::size_type i = 0; i != command_lines.size(); ++i) {
		os << "New-ADUser";
		for (std::vector<std::string>::size_type j = 0; j != command_lines[i].size(); ++j)
			os << ' ' << command_lines[i][j];
		if (i + 1 < command_lines.size())
			os << '\n';
	}

	/*
	for (std::size_t csvrow_i = 1; csvrow_i < csvrows.size(); ++csvrow_i) {
		os << "New-ADUser";
		const CsvRow &header_row = csvrows[0];
		const CsvRow &curr_csvrow = csvrows[csvrow_i];
		for (std::vector<std::string>::size_type csvfield_i = 0; csvfield_i != csvrows[csvrow_i].size(); ++csvfield_i) {
			const std::string &curr_csvfield = curr_csvrow[csvfield_i];
			const std::string &curr_header_field = header_row[csvfield_i];
			std::size_t maprow_i = 0;
			while (maprow_i != maprows.size() && maprows[maprow_i].left() != curr_header_field) {
				++maprow_i;
			}
			if (maprow_i != maprows.size()) {
				const std::string &parameter = maprows[maprow_i].right();
				const std::string &argument = curr_csvfield;
				const ValueType &type = MapRow::parameter_value.find(parameter)->second;
				if (argument.size()) {
					std::string formatted = format_argument(argument, type);
					os << ' ' << parameter << ' ' << formatted;
				}
			}
		}
		if (csvrow_i + 1 < csvrows.size())
			os << '\n';
	}
	*/
	return os;
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &a) {
	return a.print(os);
}
