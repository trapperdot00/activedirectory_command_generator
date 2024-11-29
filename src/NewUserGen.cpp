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
	return os;
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &a) {
	return a.print(os);
}
