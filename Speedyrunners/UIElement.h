#pragma once

#include "SFML/Graphics.hpp"

class UIElement : public sf::Drawable
{
protected:

	bool clickable;
	sf::FloatRect clickableBox;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

public:
	UIElement();
	UIElement(const sf::FloatRect& _clickableBox);

	void setClickableArea(const sf::FloatRect& _clickableBox);
	sf::FloatRect getClickableArea() const;

	virtual void setText(const std::string& s);

	//virtual void draw(sf::RenderWindow& window) const = 0;
	//virtual void draw(sf::RenderWindow* window) const = 0;

	virtual bool mouseInside(const sf::RenderWindow& window);
};

