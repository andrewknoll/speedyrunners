#pragma once

#include "SFML/Graphics.hpp"
#include "TileMap.h"

class Hook : public sf::Drawable, public sf::Transformable
{
protected:
	float r;
	sf::Vector2f point, vel = sf::Vector2f(0, 0); // hook point
	sf::Sprite sprite;
	sf::Vector2f hookerPosition, relPosition, offset;
	sf::FloatRect hitBox;

	bool hooked = false; // hit the wall
	bool use = false;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Hook();
	void setTextureRect(bool hooked);
	void fire(const sf::Vector2f& pos, bool facingRight);
	int update(const sf::Time& dT, const TileMap& tiles, const sf::Vector2f& hookerPosition);
	sf::Vector2f radius() const;
	float angle() const;
	sf::Vector2f tangent() const;
	void destroy();
};

