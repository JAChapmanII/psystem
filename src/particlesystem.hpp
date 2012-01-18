#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include "particle.hpp"
#include "util.hpp"

class ParticleSystem {
	public:
		ParticleSystem(ldouble idt);

		void push(Particle nparticle);
		void erase(unsigned p);

		void draw(sf::RenderTarget &target);

		void update();

		void computeCenter();
		ldouble getX();
		ldouble getY();

		unsigned size() const;
		Particle get(unsigned p) const;

	protected:
		std::vector<Particle> m_particles;
		unsigned long m_step;
		ldouble m_deltaTime;

		bool m_isDirty;
		ldouble m_cx, m_cy;
};

#endif // PARTICLE_SYSTEM_HPP
