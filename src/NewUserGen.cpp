#include "NewUserGen.h"

NewUserGen::NewUserGen(std::ifstream& csvfile, std::ifstream& mapfile, Flags f) :
	flags(f), csvrows(build_csvrows(csvfile)), maprows(build_maprows(mapfile)),
	organization_unit(get_ou()), domain_controller(get_dc())
{
	// If the csvfile is empty, or has no data aside from the header, throw
	if (csvrows.empty())
		throw std::runtime_error("csvfile is empty");
	if (csvrows.size() == 1)
		throw std::runtime_error("csvfile contains only the header");

	// If the mapfile is empty, throw
	if (maprows.size() == 0) 
		throw std::runtime_error("mapfile is empty or has only commented out rows");

	// If there are no non-command/mapping rows, throw
	if (std::count_if(maprows.cbegin(), maprows.cend(), [](const MapRow& m) { return m.type() == MapRow::mapping; }) == 0)
		throw std::runtime_error("mapfile contains only command rows");
}
	// TODO: rethrow with additional information
	// based on which initialization failed,
	// with some kind of new classes derived from
	// std::runtime_error

std::vector<CsvRow> NewUserGen::build_csvrows(std::ifstream &csvfile) {
	std::vector<CsvRow> ret;
	
	// Read lines
	std::string line;
	while (std::getline(csvfile, line)) {
		ret.emplace_back(std::move(line));
	}
	return ret;
}

std::vector<MapRow> NewUserGen::build_maprows(std::ifstream &mapfile) {
	std::vector<MapRow> ret;

	// Hold already processed parameters
	std::set<std::string> parameters;
	std::string line;
	while (std::getline(mapfile, line)) {
		// Skip commented out line
		if (commented_out(line)) continue;

		// Skip command based on flags
		if ((flags & Flags::no_overrides || flags & Flags::no_fallbacks) && is_command(line)) {
			int cmdtype = get_command_type(command_part(line));
			if ((flags & Flags::no_overrides && cmdtype == MapRow::cmd_override) ||
					(flags & Flags::no_fallbacks && cmdtype == MapRow::cmd_fallback))
				continue;
		}
		// Add a new MapRow into maprows based on the current line
		ret.emplace_back(std::move(line));
		const MapRow &maprow = ret.back();
		
		// Duplicate parameter checking for non-command lines
		if (maprow.type() == MapRow::mapping) {
			const std::string &parameter = maprow.parameter();
			if (!parameters.insert(parameter).second)
				throw std::runtime_error("multiple bindings to parameter " + parameter);
		}
	}
	return ret;
}

std::vector<std::string> NewUserGen::get_ou() {
	std::cout << "Enter Organization Unit: ";
	std::string line;
	if (!std::getline(std::cin, line))
		throw std::runtime_error("no data");
	std::vector<std::string> ret = split(line, '/');
	std::reverse(ret.begin(), ret.end());
	return ret;
}

std::vector<std::string> NewUserGen::get_dc() {
	std::cout << "Enter Domain Controller: ";
	std::string line;
	if (!std::getline(std::cin, line))
		throw std::runtime_error("no data");
	std::vector<std::string> ret = split(line, '.');
	return ret;
}

std::ostream &NewUserGen::print(std::ostream &os) const {
	// Store the line numbers of plain, mapping rows
	const auto mappers = get_mapper_rows();

	// Generate a vector of maps that hold parameter to value mappings,
	// only containing mapping rows
	auto mapped_commands = build_mapper_commands(mappers);

	// Store the line numbers of command rows
	const auto commands = command_lines();

	// Insert commands into mappings
	insert_commands(commands, mapped_commands);

	bool first_run = true;
	for (const std::map<std::string, std::string> &user_commands : mapped_commands) {
		os << (!first_run ? "\n" : "") << "New-ADUser";
		for (const auto &command : user_commands) {
			os << ' ' << command.first << ' ' << command.second;
		}
		first_run = false;
	}

	return os;
}

std::map<std::string, std::vector<std::size_t>> NewUserGen::get_mapper_rows() const {
	std::unordered_map<std::string, std::vector<std::size_t>> all;
	for (std::size_t i = 0; i != maprows.size(); ++i) {
		all[maprows[i].left()].push_back(i);
	}
	std::map<std::string, std::vector<std::size_t>> ret;
	for (const std::string &h_field : csvrows[0]) {
		std::unordered_map<std::string, std::vector<std::size_t>>::iterator it = all.find(h_field);
		if (it != all.end()) {
			ret[h_field] = std::move(it->second);
		}
	}
	return ret;
}

std::vector<std::map<std::string, std::string>>
NewUserGen::build_mapper_commands(const std::map<std::string, std::vector<std::size_t>> &mappers) const {
	
	std::vector<std::map<std::string, std::string>> commands;
	// Store seen non-unique usernames with a counter
	std::map<std::string, std::size_t> username_count;

	// Loop through CSVRow data excluding the header row
	for (std::vector<CsvRow>::const_iterator it = csvrows.cbegin() + 1; it < csvrows.cend(); ++it) {
		const CsvRow &row = *it;
		std::map<std::string, std::string> user_commands;

		for (const auto &p : mappers) {
			const std::string &header_field = p.first;
			for (const auto i : p.second) {
				std::size_t field_pos = std::find
					(csvrows[0].cbegin(), csvrows[0].cend(), header_field) - csvrows[0].cbegin();
				const std::string &parameter = maprows[i].parameter();
				if (field_pos >= row.size()) continue;
				const std::string &argument = row[field_pos];
				const ValueType &type = parameter_value.find(parameter)->second;
				if (argument.empty()) continue;
				// Check if parameter is the username one, then check for unique ones and replace them
				if (parameter == "-Name") {
					std::map<std::string, std::size_t>::const_iterator found_in_names = username_count.find(argument);
					// Set or change counter for username
					if (found_in_names == username_count.cend()) {
						username_count[argument] = 0;
					} else {
						++username_count[argument];
					}
					user_commands.insert({ parameter, make_unique_username(argument, username_count[argument])});
				} else {
					user_commands.insert({ parameter, format_argument(argument, type) });
				}
			}
		}
		// Insert -Path if not already mapped
		user_commands.insert({"-Path", format_argument(generate_path(), ValueType::String)});

		commands.push_back(user_commands);
	}
	return commands;
}

std::unordered_map<std::string, std::pair<std::string, std::vector<std::size_t>>> NewUserGen::command_lines() const {
	std::unordered_map<std::string, std::pair<std::string, std::vector<std::size_t>>> ret;
	std::size_t count = 0;
	for (const MapRow &row : maprows) {
		if (row.type() != MapRow::mapping) {
			std::pair<std::string, std::vector<std::size_t>> &sec = ret[command_part(row.left())];
			sec.first = value_part(row.left());
			sec.second.push_back(count);
		}
		++count;
	}
	return ret;
}

void NewUserGen::insert_commands
(const std::unordered_map<std::string, std::pair<std::string, std::vector<std::size_t>>> &cmd_map,
std::vector<std::map<std::string, std::string>> &mapped_commands) const {
	for (auto it = cmd_map.cbegin(); it != cmd_map.cend(); ++it) {
		const int cmd_type = get_command_type(it->first);
		for (const auto &i : it->second.second) {
			const std::string &parameter = maprows[i].parameter();
			const std::string &value = value_part(maprows[i].left());
			for (std::map<std::string, std::string> &user_commands : mapped_commands) {
				std::string formatted = format_argument(value, parameter_value.find(parameter)->second);
				switch (cmd_type) {
				case MapRow::cmd_override:
					user_commands[parameter] = formatted;
					break;
				case MapRow::cmd_fallback:
					if (user_commands.find(parameter) == user_commands.cend()
							|| user_commands.find(parameter)->second.empty())
						user_commands[parameter] = formatted;
					break;
				}
			}
		}
	}
	
	// Loop through command lines
	for (auto& user_commands : mapped_commands) {
		if (!has_all_mandatory_commands(user_commands)) {
			throw std::runtime_error("mandatory command has no value");
		}
		// Insert randomized password if flag given
		if (flags & Flags::random_password)
			randomize_password(user_commands);
	}
	
}

bool NewUserGen::has_all_mandatory_commands(const std::map<std::string, std::string> &user_commands) const {
	// All mandatory parameters
	static std::set<std::string> mandatory = { "-Name" };

	// Return if all of the mandatory fields are found in the user's commandlist
	return std::all_of(
			mandatory.cbegin(),
		   	mandatory.cend(),
		   	[&user_commands](const std::string &cmd) { return user_commands.find(cmd) != user_commands.cend(); }
	);
}

std::string
NewUserGen::make_unique_username(const std::string &username, std::size_t count) {
	static constexpr int max_digit_count = 20;
	if (count)
		return truncate_until(username, max_digit_count - digit_count(count)) + std::to_string(count);
	else
		return truncate_until(username, max_digit_count);
}

void NewUserGen::randomize_password(std::map<std::string, std::string> &user_commands) {
	// Random generators that represent the alphanumeric range
	static std::vector<Random> random_chargen =
		{ {'a', 'z'}, {'A', 'Z'}, {'0', '9'} };

	// Random generator that selects which generator to use from above vector
	static Random random_selector(0, random_chargen.size() - 1);

	// Password length
	static constexpr std::size_t pass_length = 12;

	// Parameter for user password
	static const std::string parameter = "-AccountPassword";

	// Holds the password's characters
	std::ostringstream pass_stream;

	// Generate the characters
	for (std::size_t cnt = 0; cnt != pass_length; ++cnt)
		pass_stream << static_cast<char>(random_chargen[random_selector()]());

	// Replace or add the password command parameter and formatted value into the user's commandlist
	user_commands[parameter] = format_argument(pass_stream.str(), parameter_value.find(parameter)->second);
}

std::string NewUserGen::generate_path() const {
	std::ostringstream ret;
	for (const std::string &ou : organization_unit) {
		ret << "OU=" << ou << ", ";
	}
	for (std::vector<std::string>::size_type i = 0; i != domain_controller.size(); ++i) {
		ret << "DC=" << domain_controller[i];
		// Print separator comma if current run is not the final one
		if (i + 1 != domain_controller.size())
			ret << ", ";
	}
	return ret.str();
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &a) {
	return a.print(os);
}
