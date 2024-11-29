#ifndef VALUETYPE_H
#define VALUETYPE_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <set>

enum class ValueType {
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

std::string format_argument(const std::string &, ValueType);

std::ostream &operator<<(std::ostream &, ValueType);

#endif
