#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command;

bool operator==(const Command &, const char *);
bool operator!=(const Command &, const char *);

class Command {
	friend bool operator<(const Command &, const Command &);
public:
	enum Type {
		String,
		SecureString,
		Boolean,
		DateTime,
		ADAuthenticationPolicy,
		ADAuthenticationPolicySilo,
		ADAuthType,
		X509Certificate_Array,
		PSCredential,
		ADUser,
		ADKerberosEncryptionType,
		Hashtable,
		ADPrincipal_Array,
		String_Array
	};

	Command(const std::string &s, Type t)
		: value_(s), type_(t) {}
	
	operator std::string() const {
		return value_;
	}
	Type type() const { return type_; }
	std::string name() const { return value_; }
private:
	const std::string value_;
	Type type_;
};

#endif
