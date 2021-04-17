#pragma once

#include "SFML/Graphics.hpp"
#include "UIElement.h"
#include "UISprite.h"
#include "TextElement.h"
#include "Background.h"

class Menu
{
	sf::RenderWindow* window;

	std::vector<Background> backgrounds;
	std::vector<std::unique_ptr<UIElement>> elements;


	// Main menu:
	sf::Color bgColor = sf::Color(63, 92, 123);
	std::string menuPath = glb::CONTENT_PATH + "UI/MainMenu/";

	void draw();
	void handleClick(int i);
	void pollEvents();
	void update(Settings& settings);
	//void update(Settings& settings);

public:
	//void addElement(const UIElement& e);
	//Menu(std::shared_ptr<sf::RenderWindow> _window);
	Menu(sf::RenderWindow& _window);

	Menu(sf::RenderWindow* _window);

	void addElement(std::unique_ptr<UIElement> e);

	//void addElement(std::shared_ptr<UIElement> e);

	void setMainMenu(const Settings& settings);

	//void setMainMenu();

	void loop(Settings& settings);


};

