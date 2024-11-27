#ifndef FLAGS_H
#define FLAGS_H

#include <bitset>
#include <iostream>
#include <map>

class Flags {
public:
	// Watch out for container's size in bits to be at least
	// as big as Flag's enumerator count
	enum Flag {
		random_password,
		use_overrides,
		use_fallbacks
	};

private:
	using container = std::bitset<8>;
	friend std::ostream &operator<<(std::ostream &, const Flags &);
	friend bool operator&(const Flags &, Flag);
	friend bool operator|(const Flags &, Flag);

public:
	Flags() = default;
	Flags(Flag);
	Flags(int, const char * const[]);

	Flags &operator&=(Flag);
	Flags &operator&=(const Flags &);

	Flags &operator|=(Flag);
	Flags &operator|=(const Flags &);

	Flags &operator^=(Flag);
	Flags &operator^=(const Flags &);

	Flags operator~() const;

	explicit operator bool() const { return valid; }

private:
	container data;
	bool valid = false;
	static const std::map<std::string, Flag> arg_to_flag;
	static constexpr int mandatory_arg_count = 3;
};

bool operator&(const Flags &, Flags::Flag);
bool operator|(const Flags &, Flags::Flag);
std::ostream &operator<<(std::ostream &, const Flags &);

#endif
