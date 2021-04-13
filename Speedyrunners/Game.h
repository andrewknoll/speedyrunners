#pragma once

#include "SFML/Graphics.hpp"
#include "Character.h"
#include "Level.h"
#include "Camera.h"
#include "Player.h"

class Game
{
	using CharPtr = std::shared_ptr<Character>;
public:
	enum class State { Playing, Paused, Editing };
protected:
	mutable sf::RenderWindow window;
	std::vector<CharPtr> characters;
	Camera cam;
	Level lvl;

	State state;

	float aspectRatio = 16.0 / 9.0;

	std::vector<Player> players;

	sf::Time dT; // Time since last update
	// Editor:
	//sf::Sprite selectedTile;
	Tiles::Collidable selectedTile; // When editing


	void update();

	void processEditingInputs(const sf::Event& event);

	void draw(sf::Time dT);
	void setUpWindow();

public:

	Game();

	// devuelve el indice del character en 1a pos
	int getFirstCharacterIdx() const;

	void loadLevel(const std::string& lvlPath);

	void loop();

	void addCharacter(const CharPtr character);

	void playerJoin(Player newPlayer);
};

