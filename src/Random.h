#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>
#include <cstddef>

class Random {
public:
	Random(std::size_t a, std::size_t b) 
		: e(std::chrono::steady_clock::now().time_since_epoch().count()), d(a, b)
	{}
	void seed(std::size_t n) {
		e.seed(n);
	}
	void set_range(std::size_t a, std::size_t b) {
		d = std::uniform_int_distribution<std::size_t>(a, b);
	}
	std::size_t operator()() {
		return d(e);
	}
private:
	std::default_random_engine e;
	std::uniform_int_distribution<std::size_t> d;
};

#endif
