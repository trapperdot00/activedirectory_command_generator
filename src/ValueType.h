#ifndef VALUETYPE_H
#define VALUETYPE_H

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

#endif
