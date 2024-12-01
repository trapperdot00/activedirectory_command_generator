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
		if (maprow.type() == MapRow::mapping) {
			if (existing_parameter_mappings.find(parameter) != existing_parameter_mappings.cend())
				throw std::runtime_error("multiple bindings to parameter " + parameter);
			existing_parameter_mappings.insert(parameter);
		}
	}
	if (maprows.size() == 0)
		throw std::runtime_error("mapfile is empty or has only commented out rows");
	if (std::count_if(maprows.cbegin(), maprows.cend(), [](const MapRow &m){ return m.type() == MapRow::mapping; }) == 0)
		throw std::runtime_error("mapfile contains only command rows");
}

std::ostream &NewUserGen::print(std::ostream &os) const {
	const auto mappers = get_mapper_rows();
	auto mapped_commands = build_mapper_commands(mappers);
	const auto commands = command_lines();
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
	for (std::size_t i = 0; i != maprows.size(); ++i)
		all[maprows[i].left()].push_back(i);

	std::map<std::string, std::vector<std::size_t>> ret;
	for (const std::string &h_field : csvrows[0]) {
		std::unordered_map<std::string, std::vector<std::size_t>>::iterator it = all.find(h_field);
		if (it != all.end())
			ret[h_field] = std::move(it->second);
	}
	return ret;
}

std::vector<std::map<std::string, std::string>>
NewUserGen::build_mapper_commands(const std::map<std::string, std::vector<std::size_t>> &mappers) const {
	std::vector<std::map<std::string, std::string>> commands;
	for (std::vector<CsvRow>::const_iterator it = csvrows.cbegin() + 1; it < csvrows.cend(); ++it) {
		const CsvRow &row = *it;
		std::map<std::string, std::string> user_commands;
		for (const auto &p : mappers) {
			const std::string &header_field = p.first;
			for (const auto i : p.second) {
				std::size_t field_pos = std::find
					(csvrows[0].cbegin(), csvrows[0].cend(), header_field) - csvrows[0].cbegin();
				const std::string &parameter = maprows[i].parameter();
				if (field_pos < row.size()) {
					const std::string &argument = row[field_pos];
					const ValueType &type = parameter_value.find(parameter)->second;
					if (!argument.empty())
						user_commands.insert({parameter, format_argument(argument, type)});
				}
			}
		}
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
	for (auto &m : mapped_commands) {
		if (!has_all_mandatory_commands(m))
			throw std::runtime_error("mandatory command has no value");
		if (flags & Flags::random_password)
			randomize_password(m);
	}
}

bool NewUserGen::has_all_mandatory_commands(const std::map<std::string, std::string> &user_commands) const {
	static std::set<std::string> mandatory = { "-Name" };
	return std::all_of(
			mandatory.cbegin(),
		   	mandatory.cend(),
		   	[&user_commands](const std::string &cmd) { return user_commands.find(cmd) != user_commands.cend(); }
	);
}

void NewUserGen::randomize_password(std::map<std::string, std::string> &user_commands) const {
	static std::vector<Random<char>> random_chargen =
		{ {'a', 'z'}, {'A', 'Z'}, {'0', '9'} };
	static Random<int> random_selector(0, 2);
	static constexpr std::size_t pass_length = 12;
	static const std::string parameter = "-AccountPassword";
	std::ostringstream pass_stream;
	for (std::size_t cnt = 0; cnt != pass_length; ++cnt)
		pass_stream << random_chargen[random_selector()]();
	user_commands[parameter] = format_argument(pass_stream.str(), parameter_value.find(parameter)->second);
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &a) {
	return a.print(os);
}
