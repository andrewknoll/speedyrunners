#include "Menu.h"
#include "utils.hpp"
#include "Game.h"

#ifdef USE_IMGUI
#include "imgui-SFML.h"
#endif // USE_IMGUI

//#define DEBUG_MOUSE_POS

Menu::Menu(sf::RenderWindow& _window, Settings& _settings, Game& _game) 
	: window(&_window), settings(_settings), game(_game), 
	audio(Resources::getInstance().getAudioPlayer())
{
	window->setView(window->getDefaultView());
}

Menu::Menu(sf::RenderWindow* _window, Settings& _settings, Game& _game) :
	window(_window), settings(_settings), game(_game),
	audio(Resources::getInstance().getAudioPlayer())
{
	window->setView(window->getDefaultView());
}

/*void Menu::addElement(std::unique_ptr<UIElement> e)
{
	elements.emplace_back(e);
}*/
/*VIDEOJUEGOS 2021 - GRUPO 5
Néstor Monzón (735418)
Andrés Otero (757755)
*/
#ifdef nope
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
	backgrounds.emplace_back(menuPath + "SpeedyRunnersLogo.png", *window, sf::FloatRect(0.17, 0.03, 0.73, 0.35));
	backgrounds.back().fixProportions();

	// ----------------------------- Text:
	// Multiplayer:
	sf::Vector2f pos(0.05, 0.43);
	float size = 0.05;
	//elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "", size, pos, true));
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "VIDEOJUEGOS 2021 - GRUPO 5", size, pos, false, sf::Color::White));
	

	// Story:
	pos.y += size * 2.5;
	size *= 0.85;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "Néstor Monzón (735418)", size, pos, false, sf::Color::White));
	// Practice:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "Andrés Otero (757755)", size, pos, false, sf::Color::White));
	/**
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
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "CONTROLS", size, pos, true));
	// Quit:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "QUIT", size, pos, true));
	*/
}
#endif
//#define NAH
#ifndef NAH
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
	backgrounds.emplace_back(menuPath + "SpeedyRunnersLogo.png", *window, sf::FloatRect(0.17, 0.03, 0.73, 0.35));
	backgrounds.back().fixProportions();

	// ----------------------------- Text:
	// Multiplayer:
	sf::Vector2f pos(0.05, 0.43);
	float size = 0.05;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "PLAY", size, pos, true));
	//pos.y += size * 1.25;
	//elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "VS AI", size, pos, true));

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
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "CONTROLS", size, pos, true));
	// Quit:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "QUIT", size, pos, true));
}
#endif
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
	bool active = true; int i = 0;
	for (const auto& pos : positions) { // four widgets
		widgets.emplace_back(*window, settings, lobbyPath, pos, active, i++);
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
	
	// Ready
	sf::Vector2f pos(0.05, 0.43);
	float size = 0.08;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "READY!", size, pos, true));


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

bool Menu::drawKey(int idx) const {
	return !changingControl || (idx - 2) != controlIdx;
}

void Menu::draw()
{
	window->clear(bgColor);// sf::Color(255, 0, 0));
	
	for (auto bg : backgrounds)		window->draw(bg);
	for (const auto& w : widgets)	window->draw(w);
	int idx = 0;
	for (const auto& e : elements) window->draw(*e);
	window->display();
}

void Menu::addLevels(sf::Vector2f& pos, float& size) {

	pos.y += size * 2.5;
	size *= 0.85;
	std::ifstream f(glb::LEVELS_PATH + "levels.csv");
	std::string levelName, extension = ".csv";
	while (getline(f, levelName))
	{
		if (!levelName.empty()) {
			levelNames.push_back(levelName); // Save
			// Add to UI:
			elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, levelName, size, pos, true));
			pos.y += size * 1.25;
		}
	}
}

void Menu::setWorkshopMenu() {
	currentMenuPage = Page::Workshop;
	widgets.clear();
	elements.clear();
	levelNames.clear();

	addExitSign();
	sf::Vector2f pos(0.05, 0.5);
	float size = 0.05;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "NEW", size, pos, true));

	addLevels(pos, size);
	std::vector<std::string> text{
		"RMB: move camera",
		"LMB: place selected tile",
		"Mouse wheel: change selected tile",
		"C: checkpoint mode",
		"+,-, change checkpoint size",
		"[1,2,3]: add [BoostBox,Item Pickup,Obstacle]", 
		"CTRL+L: Load last save",
		"CTRL+S: Save",
		"",
		"Bonus:",
		"Numpad7: particle system test",
		"(Numpad8 to change particles)"
	};
	pos = sf::Vector2f(0.6, 0.33);
	size = 0.04;
	std::string font = glb::CONTENT_PATH + "UI/Font/FreeSans.ttf";
	elements.emplace_back(std::make_unique<TextElement>(settings, font, "Level editor controls:", size, pos, false, sf::Color::White));
	
	pos.y += size * 2.5;
	size *= 0.85;
	for (const auto &t : text) {
		elements.emplace_back(std::make_unique<TextElement>(settings, font, t, size, pos, false, sf::Color::White));

		pos.y += size * 1.25;
	}
}

void Menu::handleMainMenuClick(int i) {
	if (i!=6) audio.play(AudioPlayer::Effect::MENU_CLICK_ITEM); // not quit
	else	  audio.play(AudioPlayer::Effect::MENU_CLICK_CANCEL); // quit
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
		game.enableCheats(true);
		game.defaultInit(2);
		exitMenu = true;
		break;
	}
	case 2:
	{
		std::cout << "Clicked practice\n";
		std::vector<glb::characterIndex> players{ glb::characterIndex::SPEEDRUNNER };
		game.enableCheats(false);
		game.defaultInit(players, std::vector<glb::characterIndex>());
		exitMenu = true;
		break;
	}
	case 3:
	{ // Workshop - Level editor
		setWorkshopMenu();
		break;
	}
	case 4:
		setControls();
		break;
	case 5: // quit
		window->close();
		exit(0);
		break; 
	default:
		std::cout << "Clicked element " << i << "\n";
	}
}

void Menu::backToMainMenu() {
	audio.play(AudioPlayer::Effect::MENU_CLICK_CANCEL);
	changingControl = false;
	clear();
	setMainMenu();
}

void Menu::handleWorkshopClick(int i) {
	if (i == 0) { // exit
		backToMainMenu();
	}
	else if (i == 1) {
		audio.play(AudioPlayer::Effect::MENU_CLICK_START_GAME);
		// New level
		game.createNewLevel(levelNames.size());
		game.clear();
		game.setState(Game::State::Editing);
		exitMenu = true;
	}
	else {
		audio.play(AudioPlayer::Effect::MENU_CLICK_START_GAME);
		i-=2;
		if (i >= levelNames.size()) std::cerr << i << " is not a level " << levelNames.size() << "\n";
		game.loadLevel(levelNames[i] + ".csv");
		game.clear();
		game.setState(Game::State::Editing);
		game.setSaveName(levelNames[i] + ".csv");
		exitMenu = true;
	}
}
void Menu::setLevelSelect() {
	widgets.clear();
	currentMenuPage = Page::LevelSelect;
	sf::Vector2f pos(0.05, 0.43);
	float size = 0.05;
	addLevels(pos, size);
}

void Menu::clearBackgrounds(int n) {
	for (int i = 0; i<n; i++) backgrounds.pop_back();
}


void Menu::setControls() {
	elements.clear();
	widgets.clear();
	addExitSign();

	// Save settings:
	// Ready
	{
		sf::Vector2f pos(0.46, 0.865);
		float size = 0.08;
		elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "SAVE", size, pos, true));
	}
	currentMenuPage = Page::Controls;
	sf::Vector2f pos(0.05, 0.43);
	float size = 0.05;

	std::vector<std::string> actions{
		"", "Left", "Right", "Slide","Jump","Grapple","Use item", "Boost"
	};
	// Single player scheme:
	std::vector<std::string> singlePlayer{
		"SINGLE PLAYER"//, "Left","Right","Down","Z","X","Spacebar"
	}; 
	settings.getControlScheme(schemes[0], 0);
	for (auto k : schemes[0]) singlePlayer.emplace_back(settings.to_string(k));
	// Multiplayer 1 scheme:
	std::vector<std::string> firstPlayer{
		"MULTIPLAYER 1"//, "A","D","S","V","B","Spacebar"
	};
	settings.getControlScheme(schemes[1], 1);
	for (auto k : schemes[1]) firstPlayer.emplace_back(settings.to_string(k));
	// Multiplayer 2 scheme:
	std::vector<std::string> secondPlayer{
		"MULTIPLAYER 2"//, "Left","Right","Down","Numpad1","Numpad2","Numpad0"
	};
	settings.getControlScheme(schemes[2], 2);
	for (auto k : schemes[2]) secondPlayer.emplace_back(settings.to_string(k));

	std::vector<std::vector<std::string>> texts{ singlePlayer, firstPlayer, secondPlayer };
	pos = sf::Vector2f(0.4, 0.38);
	size = 0.04;
	std::string font = glb::CONTENT_PATH + "UI/Font/FreeSans.ttf";
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "CONTROLS", size * 2, pos, false, sf::Color::White));

	pos.y += size * 2.6;
	float h = pos.y;
	pos.x = 0.15;// centrar opciones
	size *= 0.85;
	for (const auto& t : actions) {
		elements.emplace_back(std::make_unique<TextElement>(settings, font, t, size * 1.15, pos, false, sf::Color::White));
		pos.y += size * 1.35;
	}
	pos.x += 0.16;
	pos.y = h;
	for (const auto& v : texts) {
		bool isFirst = true;
		controlElementsFirst.push_back(elements.size()+1);
		for (const auto& t : v) {
			if (isFirst) elements.emplace_back(std::make_unique<TextElement>(settings, font, t, size, pos, false, sf::Color::White));
			else elements.emplace_back(std::make_unique<TextElement>(settings, font, t, size, pos, true));
			isFirst = false;
			pos.y += size * 1.35;
		}
		pos.x += 0.2;
		pos.y = h;
	}
}

void Menu::handleLobbyClick(int i) {
	switch (i) {
	case 0: // exit sign
		backToMainMenu();
		break;
	case 1: // Ready
	{
		audio.play(AudioPlayer::Effect::MENU_CLICK_CHARACTER_CHOSEN);
		elements.pop_back(); // remove only ready sign
		players.clear(); npcs.clear();
		int i = 0;
		for (const auto& w : widgets) {
			if (w.activated()) {
				if (i < 2) players.emplace_back(w.getSelectedCharacter());
				else npcs.emplace_back(w.getSelectedCharacter());
			}
			i++;
		}
		widgets.clear();
		clearBackgrounds(1);
		setLevelSelect();
		break;
	}
	default:
		std::cout << "Clicked element " << i << "\n";
	}
}


void Menu::handleLvlSelectClick(int i) {
	if (i == 0) { // exit
		backToMainMenu();
	}
	else {
		audio.play(AudioPlayer::Effect::MENU_CLICK_START_GAME);
		i -= 1;
		if (i >= levelNames.size()) std::cerr << i << " is not a level " << levelNames.size() << "\n";
		game.loadLevel(levelNames[i] + ".csv");
		game.setState(Game::State::Playing);
		game.enableCheats(false);
		game.setSaveName(levelNames[i] + ".csv");
		game.defaultInit(players, npcs);
		exitMenu = true;
	}
}

void Menu::saveCurrentControls() {
	audio.play(AudioPlayer::Effect::MENU_CLICK_DONE);
	//for (int i = 0; i < 3; i++) settings.setControlScheme(schemes[i], i);
	int scheme = 0, control;
	for (const auto& v : schemes) {
		control = 0;
		for (const auto& k : v)
			settings.editControl(scheme, Settings::Key(control++), k);
		scheme++;
	}
	settings.save();
}

void Menu::handleControlsClick(int i) {
	if (i == 0) { // exit
		backToMainMenu();
	}
	else if (i == 1) {
		saveCurrentControls();
		backToMainMenu();
	}
	else if (!changingControl) {
		audio.play(AudioPlayer::Effect::MENU_TOGGLE);
		elements[i]->setText("Press a key");
		elementIdx = i;
		changingControl = true;
		for (int j = 0; j < 3; j++)
			if (elementIdx >= controlElementsFirst[j]) 
				controlIdx = i - controlElementsFirst[j] + j * N_CONTROLS;
		//std::cout << "element idx: " <<elementIdx<< " controlIdx" << controlIdx << "\n";
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
	case Page::LevelSelect:
		handleLvlSelectClick(i);
		break;
	case Page::Controls:
		handleControlsClick(i);
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
		if (event.type == sf::Event::KeyPressed && event.key.code == (sf::Keyboard::Key::Escape)) {
			if (changingControl) {
				changingControl = false; 
				int scheme = controlIdx / N_CONTROLS, id = controlIdx % N_CONTROLS;
				elements[elementIdx]->setText(settings.to_string(schemes[scheme][id]));
				audio.play(AudioPlayer::Effect::MENU_CLICK_CANCEL);
			}
			else backToMainMenu();
		}
		else if (event.type == sf::Event::Closed) {
#ifdef USE_IMGUI
			ImGui::SFML::Shutdown();
#endif
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
		else if (changingControl && event.type == sf::Event::KeyPressed) {
			auto k = event.key.code;
			int scheme = controlIdx / N_CONTROLS;
			int id = controlIdx % N_CONTROLS;
			std::string asString = settings.to_string(k);
			if (asString != "") {
				schemes[scheme][id] = k;
				elements[elementIdx]->setText(asString);
				audio.play(AudioPlayer::Effect::MENU_TOGGLE);
				changingControl = false;
			}
			else { // invalid key
				audio.play(AudioPlayer::Effect::MENU_CLICK_CANCEL);
			}
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
