#include "particle.hpp"

Particle::Particle(ldouble ipx, ldouble ipy, ldouble iradius) :
		px(ipx), py(ipy), vx(0), vy(0), ax(0), ay(0), radius(iradius), mass(0) {
	mass = M_PI * radius * radius;
}

Particle::Particle(ldouble ipx, ldouble ipy, ldouble iradius, ldouble imass) :
		px(ipx), py(ipy), vx(0), vy(0), ax(0), ay(0), radius(iradius),
		mass(imass) {
}

