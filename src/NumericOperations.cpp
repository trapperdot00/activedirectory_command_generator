#include "NumericOperations.h"

std::size_t digit_count(std::size_t num) {
	std::size_t ret = 1;
	while (num /= 10)
		++ret;
	return ret;
}