#pragma once

#include "SFML/Graphics.hpp"

class UIElement
{
protected:

	bool clickable;
	sf::FloatRect clickableBox;


public:
	UIElement();
	UIElement(const sf::FloatRect& _clickableBox);

	void setClickableArea(const sf::FloatRect& _clickableBox);


	virtual void draw(sf::RenderWindow& window) const = 0;
	virtual void draw(sf::RenderWindow* window) const = 0;

	virtual bool mouseInside(const sf::RenderWindow& window);
};

