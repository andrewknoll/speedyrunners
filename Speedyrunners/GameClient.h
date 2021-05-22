#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "LobbyInterface.h"
#include "Settings.h"
#include "InGameUI.h"

class GameClient
{
	enum class State { Countdown, Playing, FinishedRound, Paused, Editing, AcceptingPlayers, Full };
protected:
	bool onlineLobby = false;
	std::shared_ptr<LobbyInterface> lobby;

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

	std::shared_ptr<sf::TcpSocket> socketToServer;
	bool cheatsEnabled = false;

	State state;
	sf::Time dT; // Time since last update


	bool testingParticles = false; // change with numpad7 in editingmode
	bool addingCheckpoint = false;

	void setUpWindow();

public:
	GameClient();

	void createLobby();

	void setFullScreen();
	void offlineUpdate(sf::Time dT);

	void setState(const State _state);
	State getState() const;

	void loopMenu();
	void loop();

	void processMouseEditing();
	void processEditingInputs(const sf::Event& event);

	void draw(sf::Time dT);
	void animateCharacters(sf::Time dT);

	// allow F1, F2, etc.
	void enableCheats(bool enable);

	void printCharacterPositions(const sf::Event& e) const;

	std::shared_ptr<LobbyInterface> createNewLevel(int nLevels);
	void setSaveName(std::string fileName);
	const Settings& getSettings() const;
};

