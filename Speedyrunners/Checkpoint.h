#pragma once

#include "SFML/Graphics.hpp"
class Checkpoint : public sf::Drawable
{
	sf::Vector2f pos;
	float r;


	// Debug:
	sf::CircleShape circle;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Checkpoint(sf::Vector2f _pos, float _r);

	void setPosition(const sf::Vector2f& _pos);

	void setRadius(const float& _r);

	sf::Vector2f getPos() const;
	float getRadius() const;
};

