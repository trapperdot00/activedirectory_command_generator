#include "ValueType.h"

std::string format_argument(const std::string &arg, ValueType type) {
	static std::set<ValueType> default_formatting_warned;
	std::ostringstream ret;
	switch (type) {
		case ValueType::String:
			ret << "\"" << arg << "\"";
			break;
		case ValueType::SecureString:
			ret << "(ConvertTo-SecureString \"" << arg << "\" -AsPlainText -Force)";
			break;
		case ValueType::Boolean:
			if (arg == "$false" || arg == "$true")
				ret << arg;
			else if (arg == "0")
				ret << "$false";
			else if (arg == "1")
				ret << "$true";
			else
				throw std::runtime_error("could not convert " + arg + " to Boolean");
			break;
		default:
			if (default_formatting_warned.find(type) == default_formatting_warned.cend()) {
				std::clog << "warning: default formatting of arguments for parameters taking " << type << '\n';
				default_formatting_warned.insert(type);
			}
			ret << arg;
			break;
	}
	return ret.str();
}

std::ostream &operator<<(std::ostream &os, const ValueType t) {
	switch (t) {
		case ValueType::String:
			os << "String";
			break;
		case ValueType::SecureString:
			os << "SecureString";
			break;
		case ValueType::Boolean:
			os << "Boolean";
			break;
		case ValueType::DateTime:
			os << "DateTime";
			break;
		case ValueType::ADAuthenticationPolicy:
			os << "ADAuthenticationPolicy";
			break;
		case ValueType::ADAuthenticationPolicySilo:
			os << "ADAuthenticationPolicySilo";
			break;
		case ValueType::ADAuthType:
			os << "ADAuthType";
			break;
		case ValueType::X509Certificate_Array:
			os << "X509Certificate_Array";
			break;
		case ValueType::PSCredential:
			os << "PSCredential";
			break;
		case ValueType::ADUser:
			os << "ADUser";
			break;
		case ValueType::ADKerberosEncryptionType:
			os << "ADKerberosEncryptionType";
			break;
		case ValueType::Hashtable:
			os << "Hashtable";
			break;
		case ValueType::ADPrincipal_Array:
			os << "ADPrincipal[]";
			break;
		case ValueType::String_Array:
			os << "String[]";
			break;
	}
	return os;
}

const std::map<std::string, ValueType> parameter_value = {
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
