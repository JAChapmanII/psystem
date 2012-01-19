#include "particlesystem.hpp"
using std::vector;
using std::string;
using std::getline;
using sf::RenderTarget;
using util::signum;

#include <iostream>
using std::cerr;
using std::endl;

#include <fstream>
using std::ifstream;

#include <limits>
using std::numeric_limits;

#include <cmath>

//using sf::CircleShape;
#include <SFML/Graphics/Shape.hpp>
using sf::Shape;

#include <SFML/Graphics/Color.hpp>
using sf::Color;

const ldouble GRAVITY = 9.8;

ldouble xAcceleration(Particle p, vector<Particle> particles);
ldouble yAcceleration(Particle p, vector<Particle> particles);

ldouble xAcceleration(Particle p, vector<Particle> particles) {
	ldouble acceleration = 0;
	for(auto i = particles.begin(); i != particles.end(); ++i) {
		ldouble dx = p.px - i->px;
		if(fabs(dx) > numeric_limits<ldouble>::epsilon())
			acceleration += i->mass / dx*dx * -signum(dx);
	}
	return acceleration * GRAVITY;
}
ldouble yAcceleration(Particle p, vector<Particle> particles) {
	ldouble acceleration = 0;
	for(auto i = particles.begin(); i != particles.end(); ++i) {
		ldouble dy = p.py - i->py;
		if(fabs(dy) > numeric_limits<ldouble>::epsilon())
			acceleration += i->mass / dy*dy * -signum(dy);
	}
	return acceleration * GRAVITY;
}

ParticleSystem::ParticleSystem(ldouble idt) : m_particles(), m_step(0),
	m_deltaTime(idt), m_isDirty(false), m_cx(0), m_cy(0) {
}

void ParticleSystem::load(string systemName) {
	ifstream in(systemName);
	if(!in.good()) {
		cerr << "ParticleSystem::load: " << systemName <<
			": does not exist" << endl;
		return;
	}
	while(!in.eof()) {
		ldouble ix, iy, iradius, imass;
		in >> ix;
		if(!in.good()) {
			if(in.eof())
				break;
			cerr << "ParticleSystem::load: error1 reading particle #"
				<< this->size() + 1 << endl;
			break;
		}
		in >> iy >> iradius >> imass;
		if(!in.good()) {
			cerr << "ParticleSystem::load: error2 reading particle #"
				<< this->size() + 1 << endl;
			break;
		}
		Particle p(ix, iy, iradius, imass);
		in >> p.vx >> p.vy >> p.ax >> p.ay;
		if(!in.good()) {
			cerr << "ParticleSystem::load: error2 reading particle #"
				<< this->size() + 1 << endl;
			break;
		}
		this->m_particles.push_back(p);
	}
	this->m_step++;
}
void ParticleSystem::push(Particle nparticle) { // {{{
	this->m_particles.push_back(nparticle);
	this->m_isDirty = true;
} // }}}
void ParticleSystem::erase(unsigned p) { // {{{
	this->m_particles.erase(this->m_particles.begin() + p);
	this->m_isDirty = true;
} // }}}

void ParticleSystem::draw(RenderTarget &target) { // {{{
	//CircleShape circle;
	//circle.SetFillColor(Color::Black);
	for(auto i = this->m_particles.begin(); i != this->m_particles.end();
			++i) {
		Shape circle = Shape::Circle(i->px, i->py, i->radius, Color::Black);
		/*
		circle.SetRadius(i->radius);
		circle.SetPosition(i->px, i->py);
		*/
		target.Draw(circle);
	}
} // }}}

void ParticleSystem::update() {
	vector<Particle> new_system;
	if(this->m_step == 0) {
		for(auto i = this->m_particles.begin(); i != this->m_particles.end();
				++i) {
			Particle next = *i;
			next.ax = xAcceleration(*i, this->m_particles) / 2.0;
			next.ay = yAcceleration(*i, this->m_particles) / 2.0;
			new_system.push_back(next);
		}
		this->m_particles = new_system;
		new_system.clear();
	}
	for(auto i = this->m_particles.begin(); i != this->m_particles.end(); ++i) {
		Particle next = *i;

		next.px += this->m_deltaTime *
			(i->vx + this->m_deltaTime / 2.0 * i->ax);
		next.py += this->m_deltaTime *
			(i->vy + this->m_deltaTime / 2.0 * i->ay);

		next.ax = xAcceleration(*i, this->m_particles);
		next.ay = yAcceleration(*i, this->m_particles);

		next.vx += this->m_deltaTime / 2.0 * (i->ax + next.ax);
		next.vy += this->m_deltaTime / 2.0 * (i->ay + next.ay);

		new_system.push_back(next);
	}
	this->m_particles = new_system;
	this->m_step++;
	this->m_isDirty = true;
}

void ParticleSystem::computeCenter() { // {{{
	this->m_isDirty = false;
	ldouble tmass = 0;
	this->m_cx = this->m_cy = 0;
	if(this->size() == 0)
		return;
	for(auto i = this->m_particles.begin(); i != this->m_particles.end();
			++i) {
		tmass += fabs(i->mass);
		this->m_cx += i->px * fabs(i->mass);
		this->m_cy += i->py * fabs(i->mass);
	}
	this->m_cx /= tmass;
	this->m_cy /= tmass;
} // }}}
ldouble ParticleSystem::getX() { // {{{
	if(this->m_isDirty)
		this->computeCenter();
	return this->m_cx;
} // }}}
ldouble ParticleSystem::getY() { // {{{
	if(this->m_isDirty)
		this->computeCenter();
	return this->m_cy;
} // }}}

unsigned ParticleSystem::size() const { // {{{
	return this->m_particles.size();
} // }}}
Particle ParticleSystem::get(unsigned p) const { // {{{
	return this->m_particles[p];
} // }}}

