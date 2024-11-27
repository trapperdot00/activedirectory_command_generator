#include "Flags.h"

Flags::Flags(Flag f) {
	data.set(f);
}

Flags::Flags(int argc, const char * const argv[]) : valid(argc > mandatory_arg_count) {
	for (int i = mandatory_arg_count + 1; i < argc; ++i) {
		std::map<std::string, Flag>::const_iterator it;
		if ((it = arg_to_flag.find(std::string(argv[i]))) != arg_to_flag.cend()) {
			data.set(it->second);
		} else {
			throw std::runtime_error("invalid option: '" + std::string(argv[i]) + "'");
		}
	}
}

Flags &Flags::operator&=(Flag f) {
	container temp;
	temp.set(f);
	data &= temp;
	return *this;
}
Flags &Flags::operator&=(const Flags &f) {
	data &= f.data;
	return *this;
}

Flags &Flags::operator|=(Flag f) {
	container temp;
	temp.set(f);
	data |= temp;
	return *this;
}
Flags &Flags::operator|=(const Flags &f) {
	data |= f.data;
	return *this;
}

Flags &Flags::operator^=(Flag f) {
	container temp;
	temp.set(f);
	data ^= temp;
	return *this;
}
Flags &Flags::operator^=(const Flags &f) {
	data ^= f.data;
	return *this;
}

Flags Flags::operator~() const {
	Flags ret;
	ret.data = ~data;
	return ret;
}

bool operator&(const Flags &lhs, Flags::Flag rhs) {
	Flags::container temp;
	temp.set(rhs);
	return (lhs.data & temp).any();
}

bool operator|(const Flags &lhs, Flags::Flag rhs) {
	Flags::container temp;
	temp.set(rhs);
	return (lhs.data | temp).any();
}

std::ostream &operator<<(std::ostream &os, const Flags &f) {
	return os << f.data;
}

const std::map<std::string, Flags::Flag> Flags::arg_to_flag = {
	{"-r", Flags::random_password}, {"--randomized-passwords", Flags::random_password},
	{"-o", Flags::use_overrides}, {"--override", Flags::use_overrides},
	{"-f", Flags::use_fallbacks}, {"--fallback", Flags::use_fallbacks}
};
constexpr int Flags::mandatory_arg_count;
