#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "util.hpp"

struct Particle {
	ldouble px, py;
	ldouble vx, vy;
	ldouble ax, ay;
	ldouble radius;
	ldouble mass;

	Particle(ldouble ipx, ldouble ipy, ldouble iradius);
	Particle(ldouble ipx, ldouble ipy, ldouble iradius, ldouble imass);
};

#endif // PARTICLE_HPP
