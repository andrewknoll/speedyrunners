#include <iostream>
#include <time.h>

#include "Game.h"
#include "utils.hpp"

Game::Game() 
	: window(sf::VideoMode(800, 800), "SpeedyRunners"),
	state(State::Editing), selectedTile(Tiles::Collidable::FLOOR), 
	dT(0)
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
	sf::Time currentTime, previousTime = clock.getElapsedTime();
	while (window.isOpen()) {
		update();
		draw();

		// https://en.sfml-dev.org/forums/index.php?topic=7018.0:
		currentTime = clock.getElapsedTime();
		dT = (currentTime.asSeconds() - previousTime.asSeconds());
		fps = 1.0f / dT; // the asSeconds returns a float
		if ((showPeriod -= dT) < 0) // Para que no este sacandolo todos los frames
		{
			showPeriod = 2; // cada 2 s
			std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
		}
		previousTime = currentTime;
	}
}

void Game::addCharacter(const Character& character)
{
	characters.emplace_back(character);
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
		if (state == State::Editing) { // Editing state
			processEditingInputs(event);
		} // End of editing state
	}
	//cam.pollEvents();
	// TODO
}

// Controls for editing state:
void Game::processEditingInputs(const sf::Event& event) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		lvl.setTile(utils::clampMouseCoord(window), selectedTile);
	}
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
			int add = -1;
			if (event.mouseWheelScroll.delta > 0) {
				add = 1;
			}
			selectedTile = (Tiles::Collidable)utils::positiveMod((selectedTile + add), Tiles::NB_COLLIDABLE);
			std::cout << "New tile selected: " << selectedTile << std::endl;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::S) && 
		lvl.save("first.csv");
	}
}


void Game::draw() const
{
	window.clear();
	window.draw(lvl);
	switch (state) {
	case State::Editing:
	{// sf::Mouse::getPosition() - window.getPosition()
		lvl.drawTile(window, sf::RenderStates(), utils::clampMouseCoord(window), selectedTile);
		break;
	}
	default:
	{
		std::cout << "unknown game state\n" << (int)state;
	}
	}
	for (auto c : characters) {
		window.draw(c);
	}
	//lvl.draw(window, cam);
	//window.draw();
	window.display();
}

