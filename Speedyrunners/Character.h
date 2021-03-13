#pragma once
#include "SFML/Graphics.hpp"
//#include <Rect.hpp>

class Character : public sf::RectangleShape
{
public:
	enum class State {Running, Sliding, InAir, Grappling, WallJumping, Stunned, Tripped};

protected:
	State state;

	sf::Rect<float> hitBox;
	//sf::RectangleShape temporaryRectangle;
	sf::Vector2f vel; // Velocity
	sf::Vector2f acc; // Acceleration

	bool isGrounded = false;
	bool hasDoubleJumped = false;

public:
	Character(sf::Rect<float> _hitBox = sf::Rect<float>(20,20,14, 30));
	void draw(sf::RenderTarget& target) const;
	void update(const sf::Time& dT);
	Character(const std::string& _path);
};

