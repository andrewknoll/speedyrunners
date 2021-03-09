#pragma once
#include "SFML/Graphics.hpp"
//#include <Rect.hpp>

class Character : public sf::RectangleShape
{
	sf::Rect<float> hitBox;
	//sf::RectangleShape temporaryRectangle;
	sf::Vector2f vel; // Velocity
	sf::Vector2f acc; // Acceleration


public:
	Character(sf::Rect<float> _hitBox = sf::Rect<float>(20,20,14, 30));
	void draw(sf::RenderTarget& target) const;
	void update(const sf::Time& dT);
	Character(const std::string& _path);
};

