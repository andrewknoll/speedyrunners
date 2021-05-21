#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "Lobby.h"
#include "Settings.h"
#include "InGameUI.h"
class GameClient
{
	enum class State { Countdown, Playing, FinishedRound, Paused, Editing, AcceptingPlayers, Full };
protected:
	bool onlineLobby = false;
	std::shared_ptr<Lobby> lobby;

	mutable sf::RenderWindow window;
	float aspectRatio = 16.0 / 9.0;

	Resources& src;

	bool running = true;
	Camera cam;
	Background bg;
	// Settings:
	Settings settings;

	// UI:
	InGameUI ui;

	// Editor:
	float currentRadius = 200.0;
	Tiles::Collidable selectedTile; 
	int selectedPSystem = 0;
	std::string saveLevelName = "defaultLevel.csv";
	Checkpoint checkpointCircle = Checkpoint(sf::Vector2f(100, 100), currentRadius);

	sf::TcpSocket socketToServer;
	bool cheatsEnabled = false;

	State state;


	bool testingParticles = false; // change with numpad7 in editingmode
	bool addingCheckpoint = false;

	void setUpWindow();

public:
	GameClient();

	void createLobby();

	void setFullScreen();
	void offlineUpdate();

	void setState(const State _state);
	State getState() const;

	void loopMenu();

	void processMouseEditing();
	void processEditingInputs(const sf::Event& event);

	void draw(sf::Time dT);
	void animateCharacters(sf::Time dT);

	// allow F1, F2, etc.
	void enableCheats(bool enable);

	void printCharacterPositions(const sf::Event& e) const;

	void createNewLevel(int nLevels);
	void setSaveName(std::string fileName);
	const Settings& getSettings() const;
};

