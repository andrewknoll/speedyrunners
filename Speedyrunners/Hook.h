#pragma once

#include "SFML/Graphics.hpp"

class Hook : public sf::Drawable
{
	float r;
	sf::Vector2f point, vel; // hook point

	bool hooked; // hit the wall
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	void fire(const sf::Vector2f& pos, bool facingRight);
	void update(const sf::Time& dT);
};

