#pragma once

#include "SFML/Graphics.hpp"

class Hook : public sf::Drawable, public sf::Transformable
{
protected:
	float r;
	sf::Vector2f point, vel = sf::Vector2f(0, 0); // hook point
	sf::Sprite sprite;
	sf::Vector2f hookerPosition, relPosition, offset;

	bool hooked; // hit the wall
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Hook();
	void fire(const sf::Vector2f& pos, bool facingRight);
	void update(const sf::Time& dT, const sf::Vector2f& hookerPosition);
};

