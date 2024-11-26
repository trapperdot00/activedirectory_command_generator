#ifndef FLAGS_H
#define FLAGS_H

#include <bitset>
#include <iostream>

class Flags {
public:
	enum Flag {
		random_password,
		b,
		c,
		d
	};

private:
	using container = std::bitset<8>;
	friend std::ostream &operator<<(std::ostream &, const Flags &);
	friend bool operator&(const Flags &, Flag);
	friend bool operator|(const Flags &, Flag);

public:
	Flags() = default;
	Flags(Flag);

	Flags &operator&=(Flag);
	Flags &operator&=(const Flags &);

	Flags &operator|=(Flag);
	Flags &operator|=(const Flags &);

	Flags &operator^=(Flag);
	Flags &operator^=(const Flags &);

	Flags operator~() const;
private:
	container data;
};

bool operator&(const Flags &, Flags::Flag);
bool operator|(const Flags &, Flags::Flag);
std::ostream &operator<<(std::ostream &, const Flags &);

#endif
