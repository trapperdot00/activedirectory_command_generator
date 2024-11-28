#include "MapRow.h"

MapRow::MapRow(const std::string &s) : Row(s), _type(mapping) {
	if (size() != 2 || only_whitespace(right()))
		throw std::runtime_error("invalid mapfile format");
	if (parameter_value.find(right()) == parameter_value.cend())
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

std::ostream &MapRow::print(std::ostream &os) const {
	return os << left() << ":\t" << right();
}

const std::map<std::string, ValueType> MapRow::parameter_value = {
	{"-AccountExpirationDate", ValueType::DateTime},
	{"-AccountNotDelegated", ValueType::Boolean},
	{"-AccountPassword", ValueType::SecureString},
	{"-AllowReversiblePasswordEncryption", ValueType::Boolean},
	{"-AuthenticationPolicy", ValueType::ADAuthenticationPolicy},
	{"-AuthenticationPolicySilo", ValueType::ADAuthenticationPolicySilo},
	{"-AuthType", ValueType::ADAuthType},
	{"-CannotChangePassword", ValueType::Boolean},
	{"-Certificates", ValueType::X509Certificate_Array},
	{"-ChangePasswordAtLogon", ValueType::Boolean},
	{"-City", ValueType::String},
	{"-Company", ValueType::String},
	{"-CompoundIdentitySupported", ValueType::Boolean},
	{"-Country", ValueType::String},
	{"-Credential", ValueType::PSCredential},
	{"-Department", ValueType::String},
	{"-Description", ValueType::String},
	{"-DisplayName", ValueType::String},
	{"-Division", ValueType::String},
	{"-EmailAddress", ValueType::String},
	{"-EmployeeID", ValueType::String},
	{"-EmployeeNumber", ValueType::String},
	{"-Enabled", ValueType::Boolean},
	{"-Fax", ValueType::String},
	{"-GivenName", ValueType::String},
	{"-HomeDirectory", ValueType::String},
	{"-HomeDrive", ValueType::String},
	{"-HomePage", ValueType::String},
	{"-HomePhone", ValueType::String},
	{"-Initials", ValueType::String},
	{"-Instance", ValueType::ADUser},
	{"-KerberosEncryptionType", ValueType::ADKerberosEncryptionType},
	{"-LogonWorkstations", ValueType::String},
	{"-Manager", ValueType::ADUser},
	{"-MobilePhone", ValueType::String},
	{"-Name", ValueType::String},
	{"-Office", ValueType::String},
	{"-OfficePhone", ValueType::String},
	{"-Organization", ValueType::String},
	{"-OtherAttributes", ValueType::Hashtable},
	{"-OtherName", ValueType::String},
	{"-PasswordNeverExpires", ValueType::Boolean},
	{"-PasswordNotRequired", ValueType::Boolean},
	{"-Path", ValueType::String},
	{"-POBox", ValueType::String},
	{"-PostalCode", ValueType::String},
	{"-PrincipalsAllowedToDelegateToAccount", ValueType::ADPrincipal_Array},
	{"-ProfilePath", ValueType::String},
	{"-SamAccountName", ValueType::String},
	{"-ScriptPath", ValueType::String},
	{"-Server", ValueType::String},
	{"-ServicePrincipalNames", ValueType::String_Array},
	{"-SmartcardLogonRequired", ValueType::Boolean},
	{"-State", ValueType::String},
	{"-StreetAddress", ValueType::String},
	{"-Surname", ValueType::String},
	{"-Title", ValueType::String},
	{"-TrustedForDelegation", ValueType::Boolean},
	{"-Type", ValueType::String},
	{"-UserPrincipalName", ValueType::String}
};
