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

struct Particle {
	typedef long double ldouble;
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
		ParticleSystem() : m_particles() {
		}

		void push(Particle nparticle) {
			this->m_particles.push_back(nparticle);
		}

		void draw(sf::RenderTarget &target) {
			//CircleShape circle;
			//circle.SetFillColor(Color::Black);
			for(auto i = this->m_particles.begin(); i != this->m_particles.end();
					++i) {
				Shape circle = Shape::Circle(
						i->px * 10, i->py * 10, i->radius * 10, Color::Black);
				/*
				circle.SetRadius(i->radius);
				circle.SetPosition(i->px, i->py);
				*/
				target.Draw(circle);
			}
		}

	protected:
		vector<Particle> m_particles;
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

	ParticleSystem psystem;
	psystem.push(Particle(0,  0, 0.3));
	psystem.push(Particle(5,  5, 0.3));
	psystem.push(Particle(5, -5, 0.3));

	bool done = false;
	while(!done && window.IsOpened()) {
		Event event;
		while(window.PollEvent(event)) {
			if(event.Type == Event::Closed)
				window.Close();
			if(event.Type == Event::KeyPressed) {
				if(event.Key.Code == Keyboard::Escape)
					done = true;
			}
		}

		view.SetCenter(0, 0);

		window.Clear(Color::White);
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

