
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "utils.hpp" // physics



Character::Character(sf::Rect<float> _hitBox) 
	: hitBox(_hitBox), 
	RectangleShape(sf::Vector2f(_hitBox.width, _hitBox.height)),
	state(State::InAir)
{
	setPosition(_hitBox.left, _hitBox.top); // Del rectangulo
	setFillColor(sf::Color::Red);
}

void Character::update(const sf::Time& dT, const TileMap& tiles)
{
	float dtSec = dT.asSeconds();

	auto collision = tiles.collision(hitBox);
	if (collision) {
		setFillColor(sf::Color::Blue);
		vel = sf::Vector2f(0, 0);
		acc = sf::Vector2f(0, 0);
	}
	else {
		// Move:
		hitBox.left += vel.x * dtSec;
		hitBox.top += vel.y * dtSec;
		setPosition(hitBox.left, hitBox.top); // Del rectangulo
		// Update vel:
		vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);

		// Update acc:
		updateAcceleration();	
		// JUST DEBUG:
		setFillColor(sf::Color::Red);

	}
	
}

void Character::setPosition(float x, float y)
{
	hitBox.left = x;
	hitBox.top = y;
	sf::RectangleShape::setPosition(x, y);
}

void Character::updateAcceleration() {
	if (state == State::InAir)
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

