#include "Menu.h"
#include "utils.hpp"
#include "Game.h"

#define DEBUG_MOUSE_POS

Menu::Menu(sf::RenderWindow& _window, Settings& _settings, Game& _game) 
	: window(&_window), settings(_settings), game(_game)
{
	window->setView(window->getDefaultView());
}

Menu::Menu(sf::RenderWindow* _window, Settings& _settings, Game& _game) :
	window(_window), settings(_settings), game(_game)
{
	window->setView(window->getDefaultView());
}

/*void Menu::addElement(std::unique_ptr<UIElement> e)
{
	elements.emplace_back(e);
}*/

void Menu::setMainMenu()
{
	currentMenuPage = Page::Main;
	clear();
	// Color plano de fondo:
	bgColor = sf::Color(0);//;63, 92, 123);
	// Sky:
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_Sky.png", *window, sf::FloatRect(0, 0, 1, 1));
	backgrounds.back().setTextureCoords(sf::FloatRect(0.1, 0.1, 0.7, 0.7)); // Recortar algo vertical
	//backgrounds.back().fixProportions();
	// Background 1:
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_far_city.png", *window, sf::FloatRect(0, 0.5, 1, 0.5)); // izq, top, anchura, altura de 0 a 1
	// Background 2 :
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_city.png", *window);// , sf::FloatRect(0, 0.5, 1, 0.5));
	backgrounds.back().setTextureCoords(sf::FloatRect(0, 0, 1, 0.5)); // Solo se muestra la mitad superior de la textura
	// Title:
	backgrounds.emplace_back(menuPath + "SpeedRunnerLogo.png", *window, sf::FloatRect(0.17, 0.03, 0.73, 0.35));
	backgrounds.back().fixProportions();

	// ----------------------------- Text:
	// Multiplayer:
	sf::Vector2f pos(0.05, 0.43);
	float size = 0.05;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "MULTIPLAYER", size, pos, true));
	// Story:
	pos.y += size*1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "DEV MODE", size, pos, true));
	// Practice:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "PRACTICE", size, pos, true));

	// Practice:
	pos.y += size * 2.5;
	size *= 0.85;
	//elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "TRAILS STORE", size, pos, true));
	// WORKSHOP:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "WORKSHOP", size, pos, true));
	// REPLAYS:
	//pos.y += size * 1.25;
	//elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "REPLAYS", size, pos, true));
	// OPTIONS:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "OPTIONS", size, pos, true));
	// Quit:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "QUIT", size, pos, true));
}
int Menu::getGameState() const
{
	return gameState;
}

void Menu::addTopLeftSign(const std::string& path) {
	backgrounds.emplace_back(path, *window, sf::FloatRect(0, 0, 0.33, 0.33));
	//backgrounds.back().fixProportions();
}


void Menu::addExitSign() {
	std::string texPath = menuPath + "SpeedRunners/Menu_Back.png";
	float size = 0.1;
	sf::Vector2f pos(0.9, 0.005);
	std::vector<sf::FloatRect> rects{ sf::FloatRect(0.005,0,1,0.5), sf::FloatRect(0.005,0.5,1,0.5) };
	elements.emplace_back(std::make_unique<UISprite>(texPath, settings, size, pos, true, rects));
}

void Menu::addLobbyWidgets(const std::string& lobbyPath) {
	std::vector<sf::Vector2f> positions;
	sf::Vector2f pos(0.355, 0.13);
	positions.push_back(pos);
	positions.push_back(pos + sf::Vector2f(0, 0.855 / 2.0));
	positions.push_back(pos + sf::Vector2f(0.316, 0));
	positions.push_back(pos + sf::Vector2f(0.316, 0.855 / 2.0));
	bool active = true;
	for (const auto& pos : positions) { // four widgets
		widgets.emplace_back(*window, settings, lobbyPath, pos, active);
		active = false;
		
	}
}


void Menu::setCharacterSelect() {
	// Path:
	std::string lobbyPath = glb::CONTENT_PATH + "UI/MultiplayerMenu/Lobby/";
	// 
	currentMenuPage = Page::Lobby;
	clear();

	// background:
	backgrounds.emplace_back(menuPath + "SpeedRunners/Menu_mid_City_B.png", *window, sf::FloatRect(0, 0, 1, 1));
	backgrounds.back().setTextureCoords(sf::FloatRect(0.01, 0, 0.99, 0.95)); // Solo se muestra la mitad superior de la textura
	

	// Exit:
	addExitSign();
	// lobby sign:
	addTopLeftSign(menuPath + "SpeedRunners/Menu_lobby.png");
	
	// character board:
	backgrounds.emplace_back(menuPath + "SpeedRunners/Menu_characterBoard.png", *window, sf::FloatRect(0.33, 0.09, 0.66, 0.9));
	//backgrounds.back().fixProportions();
	// Player widgets:
	addLobbyWidgets(lobbyPath);
	
}


void Menu::loop()
{
	while (!exitMenu) {
		update();
		draw();
	}
}

void Menu::draw()
{
	window->clear(bgColor);// sf::Color(255, 0, 0));
	
	for (auto bg : backgrounds)		window->draw(bg);
	for (const auto& w : widgets)	window->draw(w);
	for (const auto& e : elements)	window->draw(*e);
	window->display();
}


void Menu::setWorkshopMenu() {
	currentMenuPage = Page::Workshop;
	widgets.clear();
	elements.clear();
	levelNames.clear();

	addExitSign();

	std::ifstream f(glb::LEVELS_PATH + "levels.csv");
	int nLevels = 0;
	std::string levelName, extension = ".csv";

	sf::Vector2f pos(0.05, 0.43);
	float size = 0.05;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "NEW", size, pos, true));

	pos.y += size * 2.5;
	size *= 0.85;
	while (getline(f, levelName))
	{
		nLevels++;
		levelNames.push_back(levelName); // Save
		// Add to UI:
		elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, levelName, size, pos, true));

		pos.y += size * 1.25;
	}
	std::vector<std::string> text{
		"RMB: move camera",
		"LMB: place selected tile",
		"Mouse wheel: change selected tile",
		"CTRL+S: Save"
	};
	pos = sf::Vector2f(0.6, 0.33);
	size = 0.04;
	std::string font = glb::CONTENT_PATH + "UI/Font/Symbola_hint.ttf";
	elements.emplace_back(std::make_unique<TextElement>(settings, font, "Level editor controls:", size, pos, false));
	
	pos.y += size * 2.5;
	size *= 0.85;
	for (const auto &t : text) {
		elements.emplace_back(std::make_unique<TextElement>(settings, font, t, size, pos, false));

		pos.y += size * 1.25;
	}
}

void Menu::handleMainMenuClick(int i) {
	switch (i) {
	case 0:
	{
		std::cout << "Clicked multiplayer\n";
		setCharacterSelect();
		break;
	}
	case 1:
	{
		std::cout << "Clicked dev mode\n";
		game.defaultInit(2);
		game.enableCheats(true);
		exitMenu = true;
		break;
	}
	case 2:
	{
		std::cout << "Clicked practice\n";
		game.defaultInit(1);
		game.enableCheats(false);
		exitMenu = true;
		break;
	}
	case 3:
	{ // Workshop - Level editor
		setWorkshopMenu();
		/**
		game.clear();
		game.setState(Game::State::Editing);
		exitMenu = true;*/
		break;
	}
	case 5: // quit
		window->close();
		exit(0);
		break; 
	default:
		std::cout << "Clicked element " << i << "\n";
	}
}

void Menu::handleWorkshopClick(int i) {
	if (i == 0) { // exit
		clear();
		setMainMenu();
	}
	else if (i == 1) {
		// New level
		game.createNewLevel(levelNames.size());
		game.clear();
		game.setState(Game::State::Editing);
		exitMenu = true;
	}
	else {
		i-=2;
		if (i >= levelNames.size()) std::cerr << i << " is not a level " << levelNames.size() << "\n";
		game.loadLevel(levelNames[i] + ".csv");
		game.clear();
		game.setState(Game::State::Editing);
		exitMenu = true;
	}
	switch (i) {
		if (i == 0) {
			// New level
			game.createNewLevel(levelNames.size());
			game.clear();
			game.setState(Game::State::Editing);
			exitMenu = true;
		}
	case 7: // quit
		window->close();
		exit(0);
		break;
	default:
		std::cout << "Clicked element " << i << "\n";
	}
}

void Menu::handleLobbyClick(int i) {
	switch (i) {
	case 0: // exit sign
		clear();
		setMainMenu();
		break;
	default:
		std::cout << "Clicked element " << i << "\n";
	}
}

void Menu::handleClick(int i) {
	switch (currentMenuPage) {
	case Page::Main: // main menu
		handleMainMenuClick(i);
		break;
	case Page::Workshop: // 
		handleWorkshopClick(i);
		break;
	case Page::Lobby:
		handleLobbyClick(i);
		break;
	default:
		std::cerr << "what is this menu page\n";
	}
}

void Menu::pollEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			window->close();
			exit(0);
		}

		if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed) { // Click
			auto mousePos = utils::mousePosition2f(*window);
			for (int i = 0; i < elements.size(); i++) {
#ifdef DEBUG_MOUSE_POS
				if (event.type == sf::Event::MouseButtonPressed) {
					std::cout << mousePos << " rel: " << utils::globalToRelative(mousePos, *window) << "\n";
				}
#endif
				if (elements[i]->mouseInside(*window) && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
					this->handleClick(i);
					break;
				}
			}
			for (auto& w : widgets) w.update(event, mousePos);
		}
	}
}

void Menu::update()
{
	pollEvents();
}

void Menu::clear()
{
	backgrounds.clear();
	elements.clear();
	widgets.clear();
}
