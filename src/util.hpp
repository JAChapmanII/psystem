#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <vector>

// types
typedef long double ldouble;

// functions
namespace util {
	// math
	ldouble signum(ldouble &x);

	// output
	void outputHelp(std::ostream &out);

	// algorithm
	template<typename T> bool inVector(std::vector<T> vec, T val);
}

// template implementations
#include "util.imp"

#endif // UTIL_HPP
