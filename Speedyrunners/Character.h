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


	// parameters, here so they can be different e.g for bots:
	// Horizontal accelerations:
	float runningAcceleration = 200; // acceleration in the floor
	float flyingAcceleration = 100; // or in the air
	float jumpingSpeed = 200; // or in the air

	void updateAcceleration();

public:
	Character(sf::Rect<float> _hitBox = sf::Rect<float>(20,20,14, 30));
	void update(const sf::Time& dT, const TileMap& tiles);


	void setPosition(float x, float y);

	void processInputs();

	Character(const std::string& _path);
};

