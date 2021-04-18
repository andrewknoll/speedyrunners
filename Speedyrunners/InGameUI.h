#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "UISprite.h"

class Character;

class InGameUI : public sf::Drawable
{
	using CharPtr = std::shared_ptr<Character>;

	sf::RenderWindow* window;

	std::vector<std::unique_ptr<UISprite>> sprites;


	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


public:
	//InGameUI(std::vector<CharPtr> characters);
	InGameUI();
	InGameUI(sf::RenderWindow& _window);

	void setWindow(sf::RenderWindow& _window);


	void setCharacters(std::vector<CharPtr> characters);
};

