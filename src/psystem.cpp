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

	bool loadSystem = false;
	string systemName = "";
	for(unsigned i = 0; i < args.size(); ++i) {
		if(args[i] == (string)"--load") {
			if(i != args.size() - 1) {
				loadSystem = true;
				systemName = args[i + 1];
				args.erase(args.begin() + i);
				args.erase(args.begin() + i);
				i -= 2;
			} else {
				cerr << "psystem: --load requires argument" << endl;
				return 2;
			}
		}
	}

	if(!args.empty()) {
		cerr << "psystem: invalid argument passed" << endl;
		outputHelp(cerr);
		return 1;
	}

	unsigned windowWidth = 800, windowHeight = 600;
	const ldouble scaleFactor = 10.0, minPSize = 0.3, maxPSize = 3.6;
	string windowTitle = "psystem";

	RenderWindow window(VideoMode(windowWidth, windowHeight), windowTitle,
			sf::Style::Close);
	window.setFramerateLimit(60);

	View view;
	view.setSize(windowWidth / scaleFactor, windowHeight / scaleFactor);

	unsigned steps = 100;
	ParticleSystem psystem(1.0 / 60.0 / steps);

	ldouble psize = 3.6 / 4, psizeDelta = 0.1;
	if(loadSystem) {
		psystem.load(systemName);
		if(psystem.size() < 1) {
			return 3;
		}
	} else {
		unsigned ipcount = rand() % 5 + 3;
		psize = 3.6 / ipcount;
		for(unsigned i = 0; i < ipcount; ++i) {
			ldouble radians = (rand() % 360) * M_PI/180.0;
			psystem.push(Particle(cos(radians)*16.0, sin(radians)*16.0, psize));
		}
	}

	bool done = false, mode = true, run = true;
	while(!done && window.isOpen()) {
		Event event;
		while(window.pollEvent(event)) {
			if(event.type == Event::Closed)
				window.close();
			if(event.type == Event::KeyPressed) {
				if(event.key.code == Keyboard::Escape)
					done = true;
				if(event.key.code == Keyboard::Space)
					mode = !mode;
				if(event.key.code == Keyboard::P)
					run = !run;
			}
			if(event.type == Event::MouseButtonPressed) {
				// ConvertCoords -> mapPixelToCoords or mapCoordsToPixel
				sf::Vector2i mouseButtonLoc(event.mouseButton.x, event.mouseButton.y);
				ldouble mx = window.mapPixelToCoords(mouseButtonLoc, view).x,
					my = window.mapPixelToCoords(mouseButtonLoc, view).y;
				if(event.mouseButton.button == Mouse::Left) {
					psystem.push(Particle(mx, my, psize));
				} else if(event.mouseButton.button == Mouse::Right) {
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
			if(event.type == Event::MouseWheelMoved) {
				psize += event.mouseWheel.delta * psizeDelta;
				if(psize < minPSize)
					psize = minPSize;
				if(psize > maxPSize)
					psize = maxPSize;
			}
		}

		if(run)
			for(unsigned i = 0; i < steps/2; ++i)
				psystem.update();

		window.clear(Color::White);

		if(mode) {
			view.setCenter(psystem.getX(), psystem.getY());
		} else if(psystem.size() > 0) {
			view.setCenter(psystem.get(0).px, psystem.get(0).py);
		}
		window.setView(view);

		psystem.draw(window);
		window.display();
	}

	if(window.isOpen())
		window.close();

	return 0;
}

