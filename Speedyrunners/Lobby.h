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
#include "Resources.h"
#include "MusicPlayer.h"
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
	bool onlineMode = true;
	//Thread Pool
	std::vector<workerThread> threadPool = std::vector<workerThread>(12);
	std::atomic<bool> running = true;
	std::mutex finishMtx;
	std::condition_variable finishCV;

	// Main components:
	std::vector<CharPtr> characters;
	Camera cam;
	Level lvl;
	Resources& src;
	sf::Vector2f respawnPosition;


	bool gameWon = false;

	// "cheats"
	

	bool suddenDeath = false;
	State state;
	int aliveCount = 0;

	std::vector<PlayerPtr> players;
	std::vector<NPCPtr> npcs;
	std::list<ItemPtr> items;

	std::vector<particles::PSystem>& particleSystems;

	std::mutex restartMtx;
	std::condition_variable restartCv;

	sf::Time dT; // Time since last update

	//sf::Sprite selectedTile;
	
	// Checkpoints:
	std::vector<Checkpoint> checkpoints;
	int activeCheckpoint;
	// int firstCharacter;

	// Countdown:
	Countdown countdown;

	bool cheatsEnabled = false;

	std::shared_ptr<RoundVictory> rv = nullptr;

	void update();
	void updatePositions();

	// Handle an item explosion or something (when item.update returns true):
	void handleItem(Lobby::ItemPtr item);

	void clearParticles(); // clears all particles

	void updateItems();
	void updateNPCs(bool follow);

	sf::TcpListener listener;

public:

	Lobby();

	void clear();

	void requestClearParticles();

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
	void checkConnections();
	void addCharacter(const CharPtr character);

	void createNewLevel(int nLevels);

	MusicPlayer& music();

	void playerJoin(PlayerPtr newPlayer);
	void npcJoin(NPCPtr newNPC);

	CharPtr getCharacterAt(int pos) const;
	PlayerPtr getPlayerAt(int pos) const;
	NPCPtr getNPCAt(int pos) const;

	void checkItemPickups(CharPtr c);

	//OFFLINE
	void setLevelTile(const sf::Vector2i& pos, int tileNumber);
	void drawLevelTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, int tileNumber);
	void emitParticles(int selectedPSystem, const sf::Vector2f& pos, bool linear = false, int n = 1);
	const std::vector<particles::PSystem>& getParticleSystems() const;
	const std::list<ItemPtr>& getItems() const;
	const std::shared_ptr<RoundVictory>& getRV() const;
	const Countdown& getCountdown() const;
	void addCheckpoint(const sf::Vector2f& pos, float radius);
	void removeLastCheckpoint();
	void saveLevelCheckpoints();
	void enableCheats(bool enable);
	std::vector<Checkpoint> getCheckpoints() const;
	std::vector<NPCPtr> getNPCs() const;
	const std::vector<CharPtr>& getCharacters() const;
	void requestAddObject(int type, sf::Vector2f pos, CharPtr newChar = nullptr);
	void requestLoadLevel(std::string l);
	void setOffline();
	Camera getCamera() const;
	Level getLevel() const;
};

