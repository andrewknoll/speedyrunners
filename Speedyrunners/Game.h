#pragma once

#include "SFML/Graphics.hpp"
#include "Character.h"
#include "Level.h"
#include "Camera.h"

class Game
{
public:
	enum class State { Playing, Paused, Editing };
protected:
	mutable sf::RenderWindow window;
	std::vector<Character> characters;
	Camera cam;
	Level lvl;

	State state;



	sf::Time dT; // Time since last update
	// Editor:
	//sf::Sprite selectedTile;
	Tiles::Collidable selectedTile; // When editing


	void update();

	void processEditingInputs(const sf::Event& event);

	void draw() const;

public:

	Game();

	void loadLevel(const std::string& lvlPath);

	void loop();

	void addCharacter(const Character& character);

};

