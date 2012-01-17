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
	ldouble mass;
	ldouble radius;
	
	Particle(ldouble ipx, ldouble ipy, ldouble imass, ldouble iradius) :
		px(ipx), py(ipy), vx(0), vy(0), mass(imass), radius(iradius) {
	}
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

	vector<Particle> pvec;
	pvec.push_back(Particle(0,  0, 5, 0.3));
	pvec.push_back(Particle(5,  5, 5, 0.3));
	pvec.push_back(Particle(5, -5, 5, 0.3));

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

		//CircleShape circle;
		//circle.SetFillColor(Color::Black);
		for(auto i = pvec.begin(); i != pvec.end(); ++i) {
			Shape circle = Shape::Circle(
					i->px * 10, i->py * 10, i->radius * 10, Color::Black);
			/*
			circle.SetRadius(i->radius);
			circle.SetPosition(i->px, i->py);
			*/
			window.Draw(circle);
		}

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

