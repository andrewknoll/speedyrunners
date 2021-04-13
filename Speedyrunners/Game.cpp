#include <iostream>
#include <time.h>

#include "Game.h"
#include "utils.hpp"
#include "Player.h"

Game::Game() 
	: window(sf::VideoMode(1600,900), "SpeedyRunners"),
	lvl(window),
	state(State::Playing), 
	selectedTile(Tiles::Collidable::FLOOR),
	cam(sf::FloatRect(0, 0, 1600, 900))
	//dT(0)
{
	setUpWindow();
	lvl.load("first.csv", window);
}

void Game::setUpWindow() {

	window.setFramerateLimit(60); //60 FPS?
	window.setVerticalSyncEnabled(true);
}

void Game::playerJoin(Player newPlayer) {
	if (players.size() < 4) {
		players.emplace_back(newPlayer);
	}
}

int Game::getFirstCharacterIdx() const
{
	return 0;
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
		draw(dT);

		// https://en.sfml-dev.org/forums/index.php?topic=7018.0:
		currentTime = clock.getElapsedTime();
		dT = (currentTime- previousTime);
		fps = 1.0f / dT.asSeconds(); // the asSeconds returns a float
		if ((showPeriod -= dT.asSeconds()) < 0) // Para que no este sacandolo todos los frames
		{
			showPeriod = 2; // cada 2 s
			std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
		}
		previousTime = currentTime;
	}
}

void Game::addCharacter(const CharPtr character)
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
			// sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			sf::FloatRect visibleArea(0, 0, float(event.size.width), float(event.size.width) / aspectRatio);

			window.setView(sf::View(visibleArea));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
			state = State::Playing;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
			state = State::Editing;
		}
		if (state == State::Editing) { // Editing state
			processEditingInputs(event);
		} // End of editing state
		else if (state == State::Playing) { // Playing
			//characters.front().processInput(event); // Podemos cambiarlo por Player en el futuro
			for (auto& p : players) {
				p.captureEvents(event);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10)) { // Change to fullscreen (change back to window not implemented)
			sf::VideoMode vMode = sf::VideoMode::getFullscreenModes().front();
			if (!vMode.isValid()) {
				std::cerr << "NOPE\n";
			}
			window.create(vMode, "SpeedyRunners", sf::Style::Fullscreen);
			setUpWindow();
			cam = window.getDefaultView();
			lvl.loadBackground(lvl.getBackgroundPath(), window);
		}
		
	}
	if (state == State::Playing) { //
		for (auto c : characters) {
			c->update(dT, lvl.getCollidableTiles());
		}
		// ALGO ASI:
		cam.follow(characters[getFirstCharacterIdx()]->getPosition()); 
	}
	//cam.pollEvents();
	// TODO
}

// Controls for editing state:
void Game::processEditingInputs(const sf::Event& event) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		lvl.setTile(utils::clampMouseCoord(window), selectedTile);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		cam.moveByMouse(sf::Mouse::getPosition());
		//cam.move(sf::Vector2f(1, 0));
	}
	else {
		cam.resetDrag();
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::S) && 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			lvl.save("first.csv");
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			lvl.load("first.csv", window);
		}
	}
}


void Game::draw(sf::Time dT)
{
	window.clear();
	window.setView(cam);
	window.draw(lvl);
	switch (state) {
	case State::Editing:
	{// sf::Mouse::getPosition() - window.getPosition()
		lvl.drawTile(window, sf::RenderStates(), utils::clampMouseCoord(window), selectedTile);
		break;
	}
	default:
	{
		//std::cout << "unknown game state\n" << (int)state;
	}
	}
	for (auto& c : characters) {
		c->tickAnimation(dT);
		window.draw(*c);
	}
	//lvl.draw(window, cam);
	//window.draw();
	window.display();
}

