#include "particlesystem.hpp"
using std::vector;
using sf::RenderTarget;
using util::signum;

#include <limits>
using std::numeric_limits;

#include <cmath>

//using sf::CircleShape;
#include <SFML/Graphics/Shape.hpp>
using sf::Shape;

#include <SFML/Graphics/Color.hpp>
using sf::Color;

const ldouble GRAVITY = 9.8;

ParticleSystem::ParticleSystem(ldouble idt) : m_particles(), m_step(0),
	m_deltaTime(idt), m_isDirty(false), m_cx(0), m_cy(0) {
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
	for(auto i = this->m_particles.begin(); i != this->m_particles.end(); ++i) {
		Particle current = *i;
		for(auto j = this->m_particles.begin(); j != this->m_particles.end();
				++j) {
			if(i == j)
				continue;
			if(this->m_step % 2) {
				// v_{n+1} = v_n + g(t_n, x_n    ) Δt
				ldouble dx = current.px - j->px, dy = current.py - j->py;
				if(fabs(dx) > numeric_limits<ldouble>::epsilon())
					current.vx += GRAVITY * j->mass /
						dx*dx * -signum(dx) * this->m_deltaTime;
				if(fabs(dy) > numeric_limits<ldouble>::epsilon())
					current.vy += GRAVITY * j->mass /
						dy*dy * -signum(dy) * this->m_deltaTime;
				// x_{n+1} = x_n + f(t_n, v_{n+1}) Δt
				current.px += current.vx * this->m_deltaTime;
				current.py += current.vy * this->m_deltaTime;
			} else {
				// x_{n+1} = x_n + f(t_n, v_n    ) Δt
				current.px += current.vx * this->m_deltaTime;
				current.py += current.vy * this->m_deltaTime;
				// v_{n+1} = v_n + g(t_n, x_{n+1}) Δt
				ldouble dx = current.px - j->px, dy = current.py - j->py;
				if(fabs(dx) > numeric_limits<ldouble>::epsilon())
					current.vx += GRAVITY * j->mass /
						dx*dx * -signum(dx) * this->m_deltaTime;
				if(fabs(dy) > numeric_limits<ldouble>::epsilon())
					current.vy += GRAVITY * j->mass /
						dy*dy * -signum(dy) * this->m_deltaTime;
			}
		}
		new_system.push_back(current);
	}
	this->m_particles = new_system;
	this->m_step++;
	this->m_isDirty = true;
}

void ParticleSystem::computeCenter() { // {{{
	this->m_isDirty = false;
	ldouble tmass = 0;
	this->m_cx = this->m_cy = 0;
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

