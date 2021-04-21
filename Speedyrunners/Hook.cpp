#include "Hook.h"

void Hook::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void Hook::fire(const sf::Vector2f& pos, bool facingRight)
{
	//vel = sf::Vector2f(1, -1) * glb::GRAPPLING_HOOK_SPEED;
	if (!facingRight) {
		vel.x = -vel.x;
	}
}
