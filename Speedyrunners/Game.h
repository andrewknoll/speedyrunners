#pragma once

#include "SFML/Graphics.hpp"
#include "Character.h"
#include "Level.h"
#include "Camera.h"
#include "Player.h"
#include "Countdown.h"
#include "Settings.h"

class Game
{
	using CharPtr = std::shared_ptr<Character>;
	using PlayerPtr = std::shared_ptr<Player>;
public:
	enum class State { Countdown, Playing, Paused, Editing, MainMenu };
protected:
	// Settings:
	Settings settings;
	// Main components:
	mutable sf::RenderWindow window;
	std::vector<CharPtr> characters;
	Camera cam;
	Level lvl;

	State state;

	float aspectRatio = 16.0 / 9.0;

	std::vector<PlayerPtr> players;

	sf::Time dT; // Time since last update
	// Editor:
	//sf::Sprite selectedTile;
	Tiles::Collidable selectedTile; // When editing


	// Checkpoints:
	std::vector<Checkpoint> checkpoints;
	int activeCheckpoint;
	// int firstCharacter;
	float currentRadius = 200.0;

	// Countdown:
	Countdown countdown;





	void update();

	void processEditingInputs(const sf::Event& event);

	void printCharacterPositions(const sf::Event& e) const;


	void draw(sf::Time dT);
	void setUpWindow();

	void updatePositions();

public:

	Game();

	// devuelve el indice del character en 1a pos
	int getFirstCharacterIdx() const;

	void loadLevel(const std::string& lvlPath);

	void loop();

	void loopMenu();

	void addCharacter(const CharPtr character);

	void playerJoin(PlayerPtr newPlayer);
};

