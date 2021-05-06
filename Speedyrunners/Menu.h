#pragma once

#include "SFML/Graphics.hpp"
#include "UIElement.h"
#include "UISprite.h"
#include "TextElement.h"
#include "Background.h"

class Menu
{
	sf::RenderWindow* window;
	Settings& settings;

	std::vector<Background> backgrounds;
	std::vector<std::unique_ptr<UIElement>> elements;

	int currentMenuPage = 0;

	// Main menu:
	sf::Color bgColor = sf::Color(63, 92, 123);
	std::string menuPath = glb::CONTENT_PATH + "UI/MainMenu/";

	void draw();
	void handleClick(int i);
	void pollEvents();
	void update();
	//void update(Settings& settings);

	void clear();
	void addTopLeftSign(const std::string& path);
	void addExitSign();
public:
	//void addElement(const UIElement& e);
	//Menu(std::shared_ptr<sf::RenderWindow> _window);
	Menu(sf::RenderWindow& _window, Settings& _settings);

	Menu(sf::RenderWindow* _window, Settings& _settings);

	void addElement(std::unique_ptr<UIElement> e);

	//void addElement(std::shared_ptr<UIElement> e);

	void setMainMenu();

	

	void setCharacterSelect();

	//void setMainMenu();

	void loop();


};

