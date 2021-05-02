#pragma once

#include "SFML/Graphics.hpp"
#include "Character.h"
#include "Level.h"
#include "Camera.h"
#include "Player.h"
#include "Countdown.h"
#include "Settings.h"
#include "Resources.h"
#include "MusicPlayer.h"
#include "InGameUI.h"
#include "NPC.h"
#include "PlayerSlot.h"
#include <list>
#include <thread>

class Game
{
	using CharPtr = std::shared_ptr<Character>;
	using PlayerPtr = std::shared_ptr<Player>;
	using NPCPtr = std::shared_ptr<NPC>;
	using PSPtr = std::shared_ptr<PlayerSlot>;
	using ItemPtr = std::shared_ptr<Item>;

	struct Slot {
		short int type;
		short int index;
		short int controlIndex;
	};
public:
	enum class State { Countdown, Playing, Paused, Editing, MainMenu };
protected:
	//Thread Pool
	std::vector<std::unique_ptr<std::thread> > threadPool = std::vector<std::unique_ptr<std::thread> >(4);
	std::atomic<bool> running = true;
	// Settings:
	Settings settings;
	// Main components:
	mutable sf::RenderWindow window;
	std::vector<CharPtr> characters;
	Camera cam;
	Level lvl;

	Resources& src;

	State state;

	float aspectRatio = 16.0 / 9.0;

	std::vector<PlayerPtr> players;
	std::vector<NPCPtr> npcs;
	std::vector<Slot> positions;

	std::list<ItemPtr> items;

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

	// UI:
	InGameUI ui;



	void update();

	void processEditingInputs(const sf::Event& event);

	void printCharacterPositions(const sf::Event& e) const;


	void draw(sf::Time dT);
	void setUpWindow();

	void updatePositions();

public:

	Game();



	const Settings& getSettings() const;
	// devuelve el indice del character en 1a pos
	int getFirstCharacterIdx() const;

	void loadLevel(const std::string& lvlPath);

	void loop();

	void loopMenu();

	void addCharacter(const CharPtr character);

	MusicPlayer& music();

	void npcLoop();

	void playerJoin(PlayerPtr newPlayer);
	void npcJoin(NPCPtr newNPC);

	CharPtr getCharacterAt(int pos) const;
	PlayerPtr getPlayerAt(int pos) const;
	NPCPtr getNPCAt(int pos) const;
};

