#include "Game.h"
#include <iostream>
#include <time.h>

Game::Game() : window(sf::VideoMode(800, 800), "SpeedyRunners")
{
	window.setFramerateLimit(60); //60 FPS?
}

void Game::loadLevel(const std::string& lvlPath)
{
	// TODO
}

void Game::loop()
{
	// de https://en.sfml-dev.org/forums/index.php?topic=7018.0:
	float fps, showPeriod = 2;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;
	while (window.isOpen()) {
		update();
		draw();

		// https://en.sfml-dev.org/forums/index.php?topic=7018.0:
		currentTime = clock.getElapsedTime();
		float elapsed = (currentTime.asSeconds() - previousTime.asSeconds());
		fps = 1.0f / elapsed; // the asSeconds returns a float
		if ((showPeriod -= elapsed) < 0) // Para que no este sacandolo todos los frames
		{
			showPeriod = 2; // cada 2 s
			std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
		}
		previousTime = currentTime;
	}
}

void Game::update()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();
		if (event.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);

			window.setView(sf::View(visibleArea));
		}
	}
	// TODO
}

void Game::draw() const
{
	window.clear();
	window.draw(lvl);
	//lvl.draw(window, cam);
	//window.draw();
	window.display();
}
