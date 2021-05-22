#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "LobbyInterface.h"
#include "Settings.h"
#include "InGameUI.h"

class GameClient
{
protected:
	bool onlineLobby = false;
	int myID;
	std::string lobbyCode = "";
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

	sf::Time dT; // Time since last update


	bool testingParticles = false; // change with numpad7 in editingmode
	bool addingCheckpoint = false;

	void setUpWindow();

public:
	GameClient();

	void createLobby();

	void setFullScreen();
	void offlineUpdate(sf::Time dT);

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

	sf::Socket::Status connectToServer(std::string ip = "127.0.0.1");

	void disconnectFromServer();

	std::string getLobbyCode();
	int getMyId();

	void sendReadyRequest(bool r);
	void sendChangeRequest(glb::characterIndex idx, bool npc);

	//sf::Socket::Status sendRequest(OnlineRequest o) const;

	//sf::Socket::Status receiveAnswer(const void* answer);
};

