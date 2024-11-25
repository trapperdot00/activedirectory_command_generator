#include "NewUserGen.h"
#include "Command.h"

NewUserGen::NewUserGen(std::ifstream &in_file, const std::map<std::string, std::string> &c)
		: header(new CsvHeader(in_file)), header_to_cmd(validate_map(c)) {
	std::string line;
	while (std::getline(in_file, line)) {
		users.push_back(User(header, line));
	}
}

std::vector<std::string> NewUserGen::operator()() const {
	std::vector<std::string> command_vec;
	for (const User &user : users) {
		std::ostringstream build;
		build << "New-ADUser";	
		for (const std::pair<std::shared_ptr<std::string>, std::string> &p : user.values) {
			std::map<std::string, Command>::const_iterator cmd_it = header_to_cmd.find(*p.first);
			if (cmd_it != header_to_cmd.cend())
				build << " " << format_arg(cmd_it->second, p.second);
		}
		command_vec.push_back(build.str());
	}
	return command_vec;
}

std::ostream &NewUserGen::print(std::ostream &os) const {
	for (const User &user : users) {
		for (const auto &p : user.values) {
			os << *p.first << "\t:\t" << p.second << '\n';
		}
		os << '\n';
	}
	return os;
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

std::map<std::string, Command> NewUserGen::validate_map(const std::map<std::string, std::string> &m) {
	std::map<std::string, Command> ret;
	for (const std::pair<std::string, std::string> &p : m) {
		std::set<Command>::const_iterator it = commands.cbegin();
		while (it != commands.cend() && std::string(*it) != p.second) ++it;
		if (it == commands.cend())
			throw std::runtime_error("given command does not exist: " + p.second);
		ret.insert({p.first, *it});
	}
	return ret;
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
