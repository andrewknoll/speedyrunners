#pragma once
#include "SFML/Graphics.hpp"
#include "TileMap.h"
//#include <Rect.hpp>

class Character : public sf::RectangleShape
{
public:
	enum class State {Running, Sliding, InAir, Grappling, WallJumping, Stunned, Tripped};

protected:
	State state;

	sf::Rect<float> hitBox;

	sf::Vector2f vel; // Velocity
	sf::Vector2f acc; // Acceleration

	bool isGrounded = false;
	bool hasDoubleJumped = false;

	void updateAcceleration();

public:
	Character(sf::Rect<float> _hitBox = sf::Rect<float>(20,20,14, 30));
	void update(const sf::Time& dT, const TileMap& tiles);
	void draw(sf::RenderTarget& target) const;


	void setPosition(float x, float y);

	Character(const std::string& _path);
};

