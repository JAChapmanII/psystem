// std includes {{{
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <string>
using std::string;
// }}}
// SFML includes {{{
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::View;

using sf::Event;
using sf::Keyboard;
using sf::Mouse;

using sf::Color;
// }}}

#include "particle.hpp"
#include "particlesystem.hpp"
#include "util.hpp"
using util::outputHelp;
using util::inVector;

typedef long double ldouble;

int main(int argc, char **argv) {
	vector<string> args;
	for(unsigned i = 1; i < (unsigned)argc; ++i)
		args.push_back((string)argv[i]);
	srand(time(NULL));

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
	const ldouble scaleFactor = 10.0, minPSize = 0.3, maxPSize = 3.6;
	string windowTitle = "seuler";

	RenderWindow window(VideoMode(windowWidth, windowHeight), windowTitle,
			sf::Style::Close);
	window.SetFramerateLimit(60);

	View view;
	view.SetSize(windowWidth / scaleFactor, windowHeight / scaleFactor);

	unsigned steps = 100;
	ParticleSystem psystem(1.0 / 60.0 / steps);

	unsigned ipcount = rand() % 5 + 3;
	ldouble psize = 3.6 / ipcount, psizeDelta = 0.1;
	for(unsigned i = 0; i < ipcount; ++i) {
		ldouble radians = (rand() % 360) * M_PI/180.0;
		psystem.push(Particle(cos(radians)*16.0, sin(radians)*16.0, psize));
	}

	bool done = false, mode = true, run = true;
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
				if(event.Key.Code == Keyboard::P)
					run = !run;
			}
			if(event.Type == Event::MouseButtonPressed) {
				ldouble mx = window.ConvertCoords(
						event.MouseButton.X, event.MouseButton.Y, view).x,
					my = window.ConvertCoords(
						event.MouseButton.X, event.MouseButton.Y, view).y;
				if(event.MouseButton.Button == Mouse::Left) {
					psystem.push(Particle(mx, my, psize));
				} else if(event.MouseButton.Button == Mouse::Right) {
					for(unsigned p = 0; p < psystem.size(); ++p) {
						Particle cp = psystem.get(p);
						ldouble dx = mx - cp.px, dy = my - cp.py,
								distance = sqrt(dx*dx + dy*dy);
						if(distance < cp.radius) {
							psystem.erase(p);
							break;
						}
					}
				}
			}
			if(event.Type == Event::MouseWheelMoved) {
				psize += event.MouseWheel.Delta * psizeDelta;
				if(psize < minPSize)
					psize = minPSize;
				if(psize > maxPSize)
					psize = maxPSize;
			}
		}

		if(run)
			for(unsigned i = 0; i < steps/2; ++i)
				psystem.update();

		window.Clear(Color::White);

		if(mode) {
			view.SetCenter(psystem.getX(), psystem.getY());
		} else if(psystem.size() > 0) {
			view.SetCenter(psystem.get(0).px, psystem.get(0).py);
		}
		window.SetView(view);

		psystem.draw(window);
		window.Display();
	}

	if(window.IsOpened())
		window.Close();

	return 0;
}

