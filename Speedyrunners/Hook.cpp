#include "Hook.h"
#include "Resources.h"
#include "Line.hpp"
#include "utils.hpp"

Hook::Hook()
{
	sprite.setTexture(Resources::getInstance().getMiscTexture(0));
}

void Hook::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	selbaward::Line line(hookerPosition, getPosition());
	line.setThickness(4);
	line.setColor(sf::Color::Black);
	target.draw(line, states);
	target.draw(sprite, states);
}

void Hook::update(const sf::Time& dT, const sf::Vector2f& pos) {
	hookerPosition = pos + offset;
	relPosition = relPosition + vel * dT.asSeconds();
	setPosition(hookerPosition + relPosition);
	sprite.setPosition(hookerPosition + relPosition);
	sprite.setRotation(utils::degrees(atan2f(vel.y, vel.x)) + 90.0f);
}

void Hook::fire(const sf::Vector2f& pos, bool facingRight)
{
	relPosition = sf::Vector2f(0.0, 0.0);
	setPosition(relPosition);
	vel = sf::Vector2f(500, -500);
	offset = glb::FEET_TO_HAND;
	if (!facingRight) {
		vel.x = -vel.x;
		offset.x = -offset.x;
	}
}
