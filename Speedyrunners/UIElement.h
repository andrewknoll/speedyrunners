#pragma once

#include "SFML/Graphics.hpp"

class UIElement
{
protected:

	bool clickable;
	sf::IntRect clickableBox;


public:
	UIElement();
	UIElement(const sf::IntRect& _clickableBox);

	void setClickableArea(const sf::IntRect& _clickableBox);

	virtual void draw(sf::RenderWindow& window) const = 0;

	bool mouseInside() const;
};

