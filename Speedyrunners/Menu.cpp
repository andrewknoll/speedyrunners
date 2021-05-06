#include "Menu.h"
#include "utils.hpp"



Menu::Menu(sf::RenderWindow& _window, Settings& _settings) 
	: window(&_window), settings(_settings)
{
}

Menu::Menu(sf::RenderWindow* _window, Settings& _settings) :
	window(_window), settings(_settings)
{
}

/*void Menu::addElement(std::unique_ptr<UIElement> e)
{
	elements.emplace_back(e);
}*/

void Menu::setMainMenu()
{
	currentMenuPage = 0;
	clear();
	// Color plano de fondo:
	bgColor = sf::Color(0);//;63, 92, 123);
	// Sky:
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_Sky.png", *window, sf::FloatRect(0, 0, 1, 1));
	backgrounds.back().setTextureCoords(sf::FloatRect(0.05, 0.1, 0.9, 0.6)); // Recortar algo vertical
	backgrounds.back().fixProportions();
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
	std::string mainTextFontPath = glb::CONTENT_PATH + "UI/Font/Souses.ttf";
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "MULTIPLAYER", size, pos, true));
	// Story:
	pos.y += size*1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "STORY", size, pos, true));
	// Practice:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "PRACTICE", size, pos, true));

	// Practice:
	pos.y += size * 2.5;
	size *= 0.85;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "TRAILS STORE", size, pos, true));
	// WORKSHOP:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "WORKSHOP", size, pos, true));
	// REPLAYS:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "REPLAYS", size, pos, true));
	// OPTIONS:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "OPTIONS", size, pos, true));
	// Quit:
	pos.y += size * 1.25;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, "QUIT", size, pos, true));
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

	
void Menu::setCharacterSelect() {
	// Path:
	std::string path = glb::CONTENT_PATH + "UI/MultiplayerMenu/Lobby/";
	// 
	currentMenuPage = elements.size();
	clear();

	// background:
	backgrounds.emplace_back(menuPath + "SpeedRunners/Menu_mid_City_B.png", *window, sf::FloatRect(0, 0, 1, 1));
	backgrounds.back().setTextureCoords(sf::FloatRect(0.01, 0, 0.99, 0.95)); // Solo se muestra la mitad superior de la textura

	// lobby sign:
	addTopLeftSign(menuPath + "SpeedRunners/Menu_lobby.png");
	
	// character board:
	backgrounds.emplace_back(menuPath + "SpeedRunners/Menu_characterBoard.png", *window, sf::FloatRect(0.33, 0.09, 0.66, 0.9));
	//backgrounds.back().fixProportions();



	// Exit:
	addExitSign();
}


void Menu::loop()
{
	while (true) {
		update();
		draw();
	}
}

void Menu::draw()
{
	window->clear(bgColor);// sf::Color(255, 0, 0));
	for (auto bg : backgrounds) {
		//bg->draw(window);// 
		window->draw(bg);
	}
	for (const auto& e : elements) {
		window->draw(*e);
		//e->draw(window);
	}

	window->display();
}


void Menu::handleClick(int i) {
	switch (currentMenuPage + i) {
	case 0:
	{
		std::cout << "Clicked multiplayer\n";
		setCharacterSelect();
		break;
	}
	case 1:
	{
		std::cout << "Clicked story\n";
		break;
	} 
	case 8: // exit sigh
	{
		std::cout << "Clicked exit sign\n";
		setMainMenu();
		break;
	}
	default:
		std::cout << "Clicked element " << currentMenuPage + i << "\n";
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
			for (int i = 0; i < elements.size(); i++) {
				if (elements[i]->mouseInside(*window) && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
					this->handleClick(i);
					break;
				}
			}
		}
	}
}

void Menu::update()
{
	pollEvents();
	// TODO: gestion de input, cambios de estado
}

void Menu::clear()
{
	backgrounds.clear();
	elements.clear();
}
