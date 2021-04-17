#include "Menu.h"
#include "utils.hpp"



Menu::Menu(sf::RenderWindow& _window) : window(&_window)
{
}

Menu::Menu(sf::RenderWindow* _window) : window(_window)
{
}

void Menu::addElement(std::shared_ptr<UIElement> e)
{
	elements.emplace_back(e);
}

void Menu::setMainMenu(const Settings& settings)
{
	// Color plano de fondo:
	bgColor = sf::Color(63, 92, 123);
	// Sky:
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_Sky.png", *window, sf::FloatRect(0, 0, 1, 1));
	backgrounds.back().setTextureCoords(sf::FloatRect(0, 0.1, 1, 0.5)); // Recortar algo vertical
	backgrounds.back().fixProportions();
	// Background 1:
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_far_city.png", *window, sf::FloatRect(0, 0.5, 1, 0.5)); // izq, top, anchura, altura de 0 a 1
	// Background 2 :
	backgrounds.emplace_back(menuPath + "Speedrunners/Menu_city.png", *window);// , sf::FloatRect(0, 0.5, 1, 0.5));
	backgrounds.back().setTextureCoords(sf::FloatRect(0, 0, 1, 0.5)); // Solo se muestra la mitad superior de la textura
	// Title:
	backgrounds.emplace_back(menuPath + "SpeedRunnerLogo.png", *window, sf::FloatRect(0.15, 0, 0.8, 0.35));
	backgrounds.back().fixProportions();
	
	
}

void Menu::loop(Settings& settings)
{
	while (true) {
		update(settings);
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
	for (auto e : elements) {
		e->draw(*window);
	}

	window->display();
}


void Menu::handleClick(int i) {
	std::cout << "Clicked " << i << " element\n";
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
		if (event.type == sf::Event::MouseLeft) { // Click
			for (int i = 0; i < elements.size(); i++) {
				if (elements[i]->mouseInside()) {
					this->handleClick(i);
					break;
				}
			}
		}
	}
}

void Menu::update(Settings& settings)
{
	pollEvents();
	// TODO: gestion de input, cambios de estado
}
