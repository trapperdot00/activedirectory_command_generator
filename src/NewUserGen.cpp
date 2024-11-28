#include "NewUserGen.h"

NewUserGen::NewUserGen(std::ifstream &csvfile, std::ifstream &mapfile, Flags f)
	: flags(f)
{
	std::string line;
	while (std::getline(csvfile, line))
		csvrows.emplace_back(line);
	if (csvrows.empty())
		throw std::runtime_error("csvfile is empty");
	if (csvrows.size() == 1)
		throw std::runtime_error("csvfile contains only the header");

	while (std::getline(mapfile, line))
		maprows.emplace_back(line);
	if (maprows.empty())
		throw std::runtime_error("mapfile is empty");
}

std::ostream &NewUserGen::print(std::ostream &os) const {
	os << "New-ADUser ";

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
	for (const auto &p : header_field_to_map) {
		std::cout << '|' << p.first << "|\t|" << p.second << '|' << std::endl;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &a) {
	return a.print(os);
}
