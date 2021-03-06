#pragma once

#include "SFML/Graphics.hpp"
#include "Character.h"
#include "Level.h"
#include "Camera.h"

class Game
{
protected:
	mutable sf::RenderWindow window;
	std::vector<Character> characters;
	//Camera cam;
	Level lvl;


	void update();

	void draw() const;

public:
	Game();

	void loadLevel(const std::string& lvlPath);

	void loop();

};

