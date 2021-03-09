
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "utils.hpp" // physics



Character::Character(sf::Rect<float> _hitBox) : hitBox(_hitBox), RectangleShape(sf::Vector2f(_hitBox.width, _hitBox.height))
{
	setPosition(_hitBox.left, _hitBox.top); // Del rectangulo
	setFillColor(sf::Color::Red);
}

void Character::update(const sf::Time& dT)
{
	float dtSec = dT.asSeconds();
	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;
	setPosition(hitBox.left, hitBox.top); // Del rectangulo
	// Update vel:
	vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);
	// Update acc:
	acc.y = physics::GRAVITY;
}

/*Character::Character(const std::string& _path)
	: path(_path)
{
	texture.loadFromFile(_path + "animation_variant00.png");
	setTexture(texture);
	setTextureRect(sf::IntRect(300, 200, 100,100));
}*/
/*
void Character::draw(sf::RenderTarget& target) const {
	temporaryRectangle.draw(target);
}*/

