#include "NewUserGen.h"

NewUserGen::NewUserGen(std::ifstream &in_file, std::ifstream &map_file, Flags f)
		: flags(f) {
	validate_map(parse_mapfile(map_file)); // header_to_cmd
	read_rows(in_file);	// rows
	build_positions();	// valid_header_pos
}

std::ostream &NewUserGen::print(std::ostream &os) const {
	std::ostringstream stream;
	for (std::size_t i = 1; i < rows.size(); ++i) {
		stream << "New-ADUser";
		for (const std::size_t &j : valid_header_pos) {
			const std::string &header_field = rows[0][j];
			const std::string &value_field = rows[i][j];
			std::map<Command, std::string>::const_iterator f_it;
			if (flags & Flags::use_fallbacks && ((f_it = fallback_values.find(header_to_cmd.find(header_field)->second)) != fallback_values.cend()) && value_field.empty()) {
				stream << ' ' << format_arg(f_it->first, f_it->second); 
			}else {
				stream << ' ' << format_arg(header_to_cmd.find(header_field)->second, value_field);
			}
		}
		if (flags & Flags::random_password) {
			std::ostringstream pass_stream;
			static std::vector<Random<char>> randoms = {
				Random<char>('a', 'z'),
				Random<char>('A', 'Z'),
				Random<char>('0', '9')
			};
			static Random<short> r_select(0, 2);
			for (std::size_t i = 0; i != 12; ++i) {
				pass_stream << randoms[r_select()]();
			}
			stream << ' ' << format_arg({"-AccountPassword", Command::SecureString}, pass_stream.str());
		}
		stream << '\n';
	}
	return os << stream.str();
}

std::ostream &operator<<(std::ostream &os, const NewUserGen &ng) {
	return ng.print(os);
}

const std::set<Command> NewUserGen::commands = {
	{"-AccountExpirationDate", Command::DateTime},
	{"-AccountNotDelegated", Command::Boolean},
	{"-AccountPassword", Command::SecureString},
	{"-AllowReversiblePasswordEncryption", Command::Boolean},
	{"-AuthenticationPolicy", Command::ADAuthenticationPolicy},
	{"-AuthenticationPolicySilo", Command::ADAuthenticationPolicySilo},
	{"-AuthType", Command::ADAuthType},
	{"-CannotChangePassword", Command::Boolean},
	{"-Certificates", Command::X509Certificate_Array},
	{"-ChangePasswordAtLogon", Command::Boolean},
	{"-City", Command::String},
	{"-Company", Command::String},
	{"-CompoundIdentitySupported", Command::Boolean},
	{"-Country", Command::String},
	{"-Credential", Command::PSCredential},
	{"-Department", Command::String},
	{"-Description", Command::String},
	{"-DisplayName", Command::String},
	{"-Division", Command::String},
	{"-EmailAddress", Command::String},
	{"-EmployeeID", Command::String},
	{"-EmployeeNumber", Command::String},
	{"-Enabled", Command::Boolean},
	{"-Fax", Command::String},
	{"-GivenName", Command::String},
	{"-HomeDirectory", Command::String},
	{"-HomeDrive", Command::String},
	{"-HomePage", Command::String},
	{"-HomePhone", Command::String},
	{"-Initials", Command::String},
	{"-Instance", Command::ADUser},
	{"-KerberosEncryptionType", Command::ADKerberosEncryptionType},
	{"-LogonWorkstations", Command::String},
	{"-Manager", Command::ADUser},
	{"-MobilePhone", Command::String},
	{"-Name", Command::String},
	{"-Office", Command::String},
	{"-OfficePhone", Command::String},
	{"-Organization", Command::String},
	{"-OtherAttributes", Command::Hashtable},
	{"-OtherName", Command::String},
	{"-PasswordNeverExpires", Command::Boolean},
	{"-PasswordNotRequired", Command::Boolean},
	{"-Path", Command::String},
	{"-POBox", Command::String},
	{"-PostalCode", Command::String},
	{"-PrincipalsAllowedToDelegateToAccount", Command::ADPrincipal_Array},
	{"-ProfilePath", Command::String},
	{"-SamAccountName", Command::String},
	{"-ScriptPath", Command::String},
	{"-Server", Command::String},
	{"-ServicePrincipalNames", Command::String_Array},
	{"-SmartcardLogonRequired", Command::Boolean},
	{"-State", Command::String},
	{"-StreetAddress", Command::String},
	{"-Surname", Command::String},
	{"-Title", Command::String},
	{"-TrustedForDelegation", Command::Boolean},
	{"-Type", Command::String},
	{"-UserPrincipalName", Command::String}
};

std::map<std::string, std::string> NewUserGen::parse_mapfile(std::ifstream &file) {
	std::map<std::string, std::string> ret;
	std::set<std::string> existing_commands;
	std::string line;
	std::size_t lines_read = 0;
	while (std::getline(file, line)) {
		if (line.empty() || trim(line).empty()) continue;
		std::istringstream stream(line);
		std::string header, command;
		if (!std::getline(stream, header, ';') || !std::getline(stream, command)
				|| trim(header).empty() || trim(command).empty())
			throw std::runtime_error("mapfile invalid format");
		if (!is_special(header)) {
			if (existing_commands.find(command) != existing_commands.cend())
				throw std::runtime_error("multiple bindings to the same command in the mapfile");
			existing_commands.insert(command);
		}
		ret.insert({header, command});
		++lines_read;
	}
	if (lines_read == 0)
		throw std::runtime_error("mapfile empty");
	return ret;
}

void NewUserGen::read_rows(std::istream &is) {
	std::string line;
	std::size_t read_lines = 0;
	while (std::getline(is, line)) {
		rows.emplace_back(line);
		++read_lines;
	}
	if (read_lines == 0)
		throw std::runtime_error("csv file is empty");
	else if (read_lines == 1)
		throw std::runtime_error("csv file only contains the header");
}

void NewUserGen::validate_map(const std::map<std::string, std::string> &m) {
	for (const std::pair<std::string, std::string> &p : m) {
		std::set<Command>::const_iterator it = commands.cbegin();
		while (it != commands.cend() && std::string(*it) != p.second) ++it;
		if (it == commands.cend())
			throw std::runtime_error("given command does not exist: " + p.second);
		if (!is_special(p.first))
			header_to_cmd.insert({p.first, *it});
		else {
			std::string value = remove_special(p.first);
			if (value.empty())
				throw std::runtime_error("no value after special command in mapfile for " + get_special(p.first) + "|");
			if (is_override(p.first.substr(p.first.find('$')))) {
				if (flags & Flags::use_overrides)
					override_values.insert({*it, value});
			}
			else if (is_fallback(p.first.substr(p.first.find('$')))) {
				if (flags & Flags::use_fallbacks)
					fallback_values.insert({*it, value});
			} else {
				throw std::runtime_error("unrecognizable special command in mapfile: " + p.first);
			}
		}
	}
	std::cout << "Normal mappings:" << std::endl;
	for (const auto &p : header_to_cmd) {
		std::cout << '|' << p.first << '|' << '\t' << '|' << p.second.name() << '|' << std::endl;
	}
	std::cout << "\nOverride values:" << std::endl;
	for (const auto &p : override_values) {
		std::cout << '|' << p.first.name() << '|' << '\t' << '|' << p.second << '|' << std::endl;
	}
	std::cout << "\nFallback values:" << std::endl;
	for (const auto &p : fallback_values) {
		std::cout << '|' << p.first.name() << '|' << '\t' << '|' << p.second << '|' << std::endl;
	}
}

std::string NewUserGen::format_arg(const Command &cmd, const std::string &arg) {
	static std::set<std::string> default_format_warner;
	std::ostringstream stream;
	stream << std::string(cmd);
	switch (cmd.type()) {
		case Command::String:
			stream << " \"" << arg << "\"";
			break;
		case Command::SecureString:
			stream << " (ConvertTo-SecureString \"" << arg << "\" -AsPlainText -Force)";
			break;
		case Command::Boolean:
			if (arg != "0" && arg != "$false" && arg != "1" && arg != "$true")
				throw std::runtime_error("non-bool value given for parameter taking bool");
			if (arg == "0")
				stream << " $false";
			else if (arg == "1")
				stream << " $true";
			else
				stream << " " + arg;
			break;
		default:
			if (default_format_warner.find(std::string(cmd)) == default_format_warner.cend()) {
				std::cout << "warning: default formatting of parameter taking " + std::string(cmd) << std::endl;
				default_format_warner.insert(std::string(cmd));
			}
			stream << " " << arg;
			break;
	}
	return stream.str();
}

void NewUserGen::build_positions() {
	if (!rows.empty()) {
		for (std::size_t i = 0; i != rows[0].size(); ++i) {
			std::map<std::string, Command>::const_iterator it;
			if ((it = header_to_cmd.find(rows[0][i])) == header_to_cmd.cend())
				continue;
			if ((flags & Flags::random_password) && std::string(it->second) == "-AccountPassword")
				continue;
			valid_header_pos.push_back(i);
		}
	}
}

bool NewUserGen::is_special(const std::string &s) {
	if (s.find('$') != std::string::npos) {
		return true;
	}
	return false;
}

bool NewUserGen::is_override(const std::string &s) {
	return s.find("$OVERRIDE$") == 0;
}

bool NewUserGen::is_fallback(const std::string &s) {
	return s.find("$FALLBACK$") == 0;
}

std::string NewUserGen::remove_special(const std::string &s) {
	std::size_t cnt = 0;
	std::string::size_type i = 0;
	while (i != s.size() && cnt != 2) {
		if (s[i++] == '$') ++cnt;
	}
	if (i != s.size())
		return s.substr(i);
	else
		return "";
}

std::string NewUserGen::get_special(const std::string &s) {
	std::string::size_type beg_pos = 0;
	bool encountered = false;
	while (beg_pos != s.size() && !encountered) {
		if (s[beg_pos] == '$')
			encountered = true;
		else
			++beg_pos;
	}
	if (!encountered)
		return "";

	std::string::size_type end_pos = beg_pos + 1;
	encountered = false;
	while (end_pos != s.size() && !encountered) {
		if (s[end_pos] == '$')
			encountered = true;
		else
			++end_pos;
	}
	if (!encountered)
		return "";

	return s.substr(beg_pos, end_pos - beg_pos + 1);
}
