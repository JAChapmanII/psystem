#include "util.hpp"
using std::ostream;
using std::endl;

#include <cmath>

void util::outputHelp(ostream &out) {
	out << "psystem: An implementation of a simplectic Euler integrator" << endl;
	out << "\t-h|--help: print this help message and quit" << endl;
	out << "see the README for more information" << endl;
}

ldouble util::signum(ldouble &x) {
	if(x < 0)
		return -1;
	return 1;
}

