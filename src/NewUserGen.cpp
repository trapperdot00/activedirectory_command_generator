#include "NewUserGen.h"

NewUserGen::NewUserGen(std::ifstream &csvfile, std::ifstream &mapfile, Flags f)
	: flags(f)
{
	for (std::string line; std::getline(csvfile, line); csvrows.emplace_back(line)) ;
	if (csvrows.empty())
		throw std::runtime_error("csvfile is empty");
	if (csvrows.size() == 1)
		throw std::runtime_error("csvfile contains only the header");

	for (std::string line; std::getline(mapfile, line); maprows.emplace_back(line)) ;
	if (maprows.empty())
		throw std::runtime_error("mapfile is empty");
}

std::ostream &NewUserGen::print(std::ostream &os) const {

	// build map
	std::multimap<std::string, std::string> header_field_to_map;
	const CsvRow &header_row = csvrows[0];
	for (std::vector<std::string>::size_type i = 0; i != header_row.size(); ++i) {
		for (const MapRow &map_row : maprows) {
			if (map_row.type() == MapRow::mapping && header_row[i] == map_row.left()) {
				header_field_to_map.insert({header_row[i], map_row.right()});
			}
		}
	}

	// print
	for (std::size_t row_index = 1; row_index < csvrows.size(); ++row_index) {
		os << "New-ADUser";
		const CsvRow &current_row = csvrows[row_index];
		for (const auto &p : header_field_to_map) {
			std::vector<std::string>::const_iterator found = std::find(header_row.cbegin(), header_row.cend(), p.first);
			std::vector<std::string>::difference_type field_num = found - header_row.cbegin();
			const ValueType &type = MapRow::parameter_value.find(p.second)->second;

			const std::string &parameter = p.second;
			const std::string &argument = current_row[field_num];
			os << " " << parameter << " " << format_argument(argument, type);
		}
		if (row_index + 1 != csvrows.size())
			os << '\n';
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &a) {
	return a.print(os);
}
