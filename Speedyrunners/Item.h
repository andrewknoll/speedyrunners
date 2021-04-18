#pragma once
#include "SFML/Graphics.hpp"
class Item : public sf::Drawable
{
private:
	sf::Sprite icon;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
public:
	virtual void update(sf::Time elapsed) = 0;
};

