// std includes {{{
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <algorithm>
using std::find;

#include <cmath>
// }}}
// SFML includes {{{
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::View;

using sf::Event;
using sf::Keyboard;

using sf::Color;
using sf::Shape;
//using sf::CircleShape;
// }}}

void outputHelp(ostream &out);
template<typename T> bool inVector(vector<T> vec, T val);
long double signum(long double &x);

typedef long double ldouble;
const ldouble GRAVITY = 9.8;
const ldouble scaleFactor = 10.0;

struct Particle {
	ldouble px, py;
	ldouble vx, vy;
	ldouble radius;
	ldouble mass;

	Particle(ldouble ipx, ldouble ipy, ldouble iradius) :
			px(ipx), py(ipy), vx(0), vy(0), radius(iradius), mass(0) {
		mass = M_PI * radius * radius;
	}

	Particle(ldouble ipx, ldouble ipy, ldouble iradius, ldouble imass) :
			px(ipx), py(ipy), vx(0), vy(0), radius(iradius), mass(imass) {
	}
};

class ParticleSystem {
	public:
		ParticleSystem(ldouble idt) : m_particles(), m_step(0), m_deltaTime(idt),
				m_isDirty(false), m_cx(0), m_cy(0) {
		}

		void push(Particle nparticle) {
			this->m_particles.push_back(nparticle);
			this->m_isDirty = true;
		}

		void draw(sf::RenderTarget &target) {
			//CircleShape circle;
			//circle.SetFillColor(Color::Black);
			for(auto i = this->m_particles.begin(); i != this->m_particles.end();
					++i) {
				Shape circle = Shape::Circle(
						i->px * scaleFactor, i->py * scaleFactor,
						i->radius * scaleFactor, Color::Black);
				/*
				circle.SetRadius(i->radius);
				circle.SetPosition(i->px, i->py);
				*/
				target.Draw(circle);
			}
		}

		void update() {
			vector<Particle> new_system;
			for(auto i = this->m_particles.begin(); i != this->m_particles.end();
					++i) {
				Particle current = *i;
				for(auto j = this->m_particles.begin();
						j != this->m_particles.end(); ++j) {
					if(i == j)
						continue;
					if(this->m_step % 2) {
						// v_{n+1} = v_n + g(t_n, x_n    ) Δt
						ldouble dx = current.px - j->px, dy = current.py - j->py;
						current.vx += GRAVITY * j->mass /
							dx*dx * -signum(dx) * this->m_deltaTime;
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
						current.vx += GRAVITY * j->mass /
							dx*dx * -signum(dx) * this->m_deltaTime;
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

		void computeCenter() {
			this->m_isDirty = false;
			ldouble tmass = 0;
			this->m_cx = this->m_cy = 0;
			for(auto i = this->m_particles.begin(); i != this->m_particles.end();
					++i) {
				tmass += i->mass;
				this->m_cx += i->px * i->mass;
				this->m_cy += i->py * i->mass;
			}
			this->m_cx /= tmass;
			this->m_cy /= tmass;
		}

		ldouble getX() {
			if(this->m_isDirty)
				this->computeCenter();
			return this->m_cx;
		}

		ldouble getY() {
			if(this->m_isDirty)
				this->computeCenter();
			return this->m_cy;
		}

		unsigned size() const {
			return this->m_particles.size();
		}

		Particle get(unsigned p) const {
			return this->m_particles[p];
		}

	protected:
		vector<Particle> m_particles;
		unsigned long m_step;
		ldouble m_deltaTime;

		bool m_isDirty;
		ldouble m_cx, m_cy;
};

int main(int argc, char **argv) {
	vector<string> args;
	for(unsigned i = 1; i < (unsigned)argc; ++i)
		args.push_back((string)argv[i]);

	if(inVector(args, (string)"--help") || inVector(args, (string)"-h")) {
		outputHelp(cout);
		return 0;
	}
	if(!args.empty()) {
		cerr << "seuler: invalid argument passed" << endl;
		outputHelp(cerr);
		return 1;
	}

	unsigned windowWidth = 800, windowHeight = 600;
	string windowTitle = "seuler";
	RenderWindow window(VideoMode(windowWidth, windowHeight), windowTitle,
			sf::Style::Close);
	window.SetFramerateLimit(60);

	View view;
	view.SetSize(windowWidth, windowHeight);

	unsigned steps = 100;
	ParticleSystem psystem(1.0 / 60.0 / steps);
	psystem.push(Particle(-5,   0, 0.9));
	psystem.push(Particle(10,   5, 0.9));
	psystem.push(Particle( 7, - 7, 0.9));
	psystem.push(Particle(-7, -13, 0.9));

	bool done = false, mode = true;
	ldouble xo = 0, yo = 0;
	while(!done && window.IsOpened()) {
		Event event;
		while(window.PollEvent(event)) {
			if(event.Type == Event::Closed)
				window.Close();
			if(event.Type == Event::KeyPressed) {
				if(event.Key.Code == Keyboard::Escape)
					done = true;
				if(event.Key.Code == Keyboard::Space)
					mode = !mode;
			}
		}

		for(unsigned i = 0; i < steps; ++i)
			psystem.update();

		window.Clear(Color::White);

		if(mode)
			view.SetCenter(psystem.getX() * scaleFactor,
					psystem.getY() * scaleFactor);
		else
			view.SetCenter(psystem.get(0).px * scaleFactor,
					psystem.get(0).py * scaleFactor);
		window.SetView(view);

		psystem.draw(window);
		window.Display();
	}

	if(window.IsOpened())
		window.Close();

	return 0;
}

void outputHelp(ostream &out) {
	out << "seuler: An implementation of a simplectic Euler integrator" << endl;
	out << "\t-h|--help: print this help message and quit" << endl;
}

template<typename T> bool inVector(vector<T> vec, T val) {
	return (find(vec.begin(), vec.end(), val) != vec.end());
}

long double signum(long double &x) {
	if(x < 0)
		return -1;
	return 1;
}

