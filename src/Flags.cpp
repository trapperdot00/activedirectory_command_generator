#include "Flags.h"

Flags::Flags(Flag f) {
	data.set(f);
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
