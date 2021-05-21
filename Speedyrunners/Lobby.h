#pragma once

#include <list>
#include <thread>
#include <condition_variable>
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
#include "RoundVictory.h"
#include "SFML/Network.hpp"

#include "ParticleSystem.h"

// Defines de configuracion para debug:
//#define USE_IMGUI
#define DEV_MODE
#define DISABLE_FULLSCREEN

//#define VERBOSE_DEBUG // Cambiar para quitar couts




class Lobby
{
	using CharPtr = std::shared_ptr<Character>;
	using PlayerPtr = std::shared_ptr<Player>;
	using NPCPtr = std::shared_ptr<NPC>;
	using PSPtr = std::shared_ptr<PlayerSlot>;
	using ItemPtr = std::shared_ptr<Item>;

	struct workerThread {
		std::unique_ptr<std::thread> threadPtr;
		bool finished = true;
	};

public:
	enum class State { Countdown, Playing, FinishedRound, Paused, Editing, AcceptingPlayers, Full };
protected:

	//Thread Pool
	std::vector<workerThread> threadPool = std::vector<workerThread>(12);
	std::atomic<bool> running = true;
	std::mutex finishMtx;
	std::condition_variable finishCV;
	// Settings:
	Settings settings;
	// Main components:
	mutable sf::RenderWindow window;
	std::vector<CharPtr> characters;
	Camera cam;
	Level lvl;
	Resources& src;
	sf::Vector2f respawnPosition;


	bool gameWon = false;


	std::string saveLevelName = "defaultLevel.csv";

	// "cheats"
	bool cheatsEnabled = false;

	bool suddenDeath = false;
	State state;
	float aspectRatio = 16.0 / 9.0;
	int aliveCount = 0;

	std::vector<PlayerPtr> players;
	std::vector<NPCPtr> npcs;
	std::list<ItemPtr> items;

	std::vector<particles::PSystem>& particleSystems;

	std::mutex restartMtx;
	std::condition_variable restartCv;

	sf::Time dT; // Time since last update

	// Checkpoints:
	// Editor:
	float currentRadius = 200.0;
	//sf::Sprite selectedTile;
	Tiles::Collidable selectedTile; // When editing
	bool addingCheckpoint = false;
	Checkpoint checkpointCircle = Checkpoint(sf::Vector2f(100,100), currentRadius);
	// Checkpoints:
	std::vector<Checkpoint> checkpoints;
	int activeCheckpoint;
	// int firstCharacter;

	bool testingParticles = false; // change with numpad7 in editingmode
	int selectedPSystem = 0;
	// Countdown:
	Countdown countdown;

	std::unique_ptr<RoundVictory> rv = nullptr;

	// UI:
	InGameUI ui;

	void update();
	void processEditingInputs(const sf::Event& event);
	void printCharacterPositions(const sf::Event& e) const;
	void draw(sf::Time dT);
	void setUpWindow();
	void updatePositions();

	// Handle an item explosion or something (when item.update returns true):
	void handleItem(Lobby::ItemPtr item);
	void clearParticles(); // clears all particles

	void processMouseEditing();

	void animateCharacters();

	void updateItems();
	void updateNPCs(bool follow);

	sf::TcpListener listener;

public:

	Lobby();

	void clear();

	void defaultInit(int N_PLAYERS);
	void defaultInit(const std::vector<glb::characterIndex>& _players, const std::vector<glb::characterIndex>& _npcs);
	void setState(const State _state);
	State getState() const;
	
	const Settings& getSettings() const;
	// devuelve el indice del character en 1a pos
	int getFirstCharacterIdx() const;
	void loadLevel(const std::string& lvlPath);
	void setSaveName(std::string fileName);
	void loopGame();
	void loopMenu();
	void checkConnections();
	void addCharacter(const CharPtr character);

	void createNewLevel(int nLevels);

	MusicPlayer& music();

	void setFullScreen();

	void playerJoin(PlayerPtr newPlayer);
	void npcJoin(NPCPtr newNPC);

	CharPtr getCharacterAt(int pos) const;
	PlayerPtr getPlayerAt(int pos) const;
	NPCPtr getNPCAt(int pos) const;

	void checkItemPickups(CharPtr c);

	// allow F1, F2, etc.
	void enableCheats(bool enable);
};

