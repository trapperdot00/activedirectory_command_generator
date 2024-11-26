#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>
#include <cstddef>

template <typename T>
class Random {
public:
	using value_type = T;
	Random(const value_type &a, const value_type &b) 
		: e(std::chrono::steady_clock::now().time_since_epoch().count()), d(a, b)
	{}
	void seed(std::size_t n) {
		e.seed(n);
	}
	void set_range(const value_type &a, const value_type &b) {
		d = std::uniform_int_distribution<value_type>(a, b);
	}
	value_type operator()() {
		return d(e);
	}
private:
	std::default_random_engine e;
	std::uniform_int_distribution<value_type> d;
};

template <>
class Random<bool> {
public:
	using value_type = bool;
	Random(double p = 0.5)
		: e(std::chrono::steady_clock::now().time_since_epoch().count()), d(p) {}
	void seed(std::size_t n) {
		e.seed(n);
	}
	void set_range(double p) {
		d = std::bernoulli_distribution(p);
	}
	value_type operator()() {
		return d(e);
	}
private:
	std::default_random_engine e;
	std::bernoulli_distribution d;
};

#endif
