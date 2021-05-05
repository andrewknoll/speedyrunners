#pragma once
#include "SFML/Graphics.hpp"
class SuddenDeathBackground : public sf::View, public sf::Drawable
{
private:
	sf::RectangleShape coloredRect;
	bool increasing = true;
	bool active = true;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	SuddenDeathBackground(sf::FloatRect rect);
	void update(sf::RenderTarget& target);
	void setActive(bool active);
	sf::RectangleShape getRectangle() const;
};

