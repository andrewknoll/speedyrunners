#pragma once

#include "SFML/Graphics.hpp"
#include "UIElement.h"
#include "UISprite.h"
#include "TextElement.h"
#include "Background.h"
#include "LobbyWidget.h"
#include "AudioPlayer.h"

class GameServer;

class Menu
{
	enum class Page {
		Main,
		Workshop,
		Lobby,
		LevelSelect,
		Controls
	};

	GameServer& game;
	sf::RenderWindow* window;
	Settings& settings;
	
	bool exitMenu = false;
	int gameState = 0;

	std::vector<Background> backgrounds;
	std::vector<std::unique_ptr<UIElement>> elements;
	std::vector<LobbyWidget> widgets;

	
	int nPlayers = 1;

	std::vector<std::string> levelNames;

	std::vector<glb::characterIndex> players;
	std::vector<glb::characterIndex> npcs;

	Page currentMenuPage = Page::Main;

	// Main menu:
	sf::Color bgColor = sf::Color(63, 92, 123);
	std::string menuPath = glb::CONTENT_PATH + "UI/MainMenu/";
	std::string mainTextFontPath = glb::CONTENT_PATH + "UI/Font/Souses.ttf";

	// Controls:
	std::array<std::vector<sf::Keyboard::Key>, 3> schemes; 
	bool changingControl = false;
	int controlIdx = 0, elementIdx=0;
	std::vector<int> controlElementsFirst;

	// Audio:
	AudioPlayer& audio;

	void draw();
	void addLevels(sf::Vector2f& pos, float& size);
	void handleLobbyClick(int i);
	void handleLvlSelectClick(int i);
	void handleClick(int i);
	void pollEvents();
	void update();
	//void update(Settings& settings);
	void setWorkshopMenu();
	void handleMainMenuClick(int i);
	void handleWorkshopClick(int i);
	void setLevelSelect();
	void clearBackgrounds(int n); // clears n backgrounds
	void setCharacterSelect();
	void setControls();

	bool drawKey(int idx) const;

	void clear();
	void addTopLeftSign(const std::string& path);
	void addExitSign();
	void addLobbyWidgets(const std::string& lobbyPath);
	void backToMainMenu();
	void handleControlsClick(int i);
	void saveCurrentControls();
public:
	//void addElement(const UIElement& e);
	//Menu(std::shared_ptr<sf::RenderWindow> _window);
	Menu(sf::RenderWindow& _window, Settings& _settings, GameServer& _game);

	Menu(sf::RenderWindow* _window, Settings& _settings, GameServer& _game);

	void addElement(std::unique_ptr<UIElement> e);

	//void addElement(std::shared_ptr<UIElement> e);

	void setMainMenu();

	int getGameState() const;

	


	//void setMainMenu();

	void loop();


};

