#include "Rocket.h"
#include "Resources.h"
#include "utils.hpp"
#include <cmath>

Rocket::Rocket(sf::Vector2f pos, CharPtr target, bool facingRight) :
	target(target),
	particles(3),
	position(pos)
{
	rocket.setTexture(Resources::getInstance().getItemTexture(glb::item::ROCKET));
	
	if (facingRight) {
		vel = sf::Vector2f(velValIni, -velValIni);
	}
	else {
		vel = sf::Vector2f(-velValIni, -velValIni);
	}
}

void Rocket::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(particles, states);
	target.draw(rocket, states);
}

void Rocket::update(sf::Time elapsed) {
	sf::Vector2f diff;
	//Update Acceleration
	if (target != nullptr) {
		diff = (target->getPosition() - position);
		acc.x = accVal * (diff.x);
		acc.y = accVal * (diff.y);
		/*if (diff.x > 0) acc.x = accVal;
		else acc.x = -accVal;
		if (diff.y > 0) acc.y = accVal;
		else acc.y = -accVal;*/
	}

	//Move
	position += vel * elapsed.asSeconds();
	particles.setEmitter(position);

	//Update velocity and angle
	vel = vel + acc * elapsed.asSeconds();
	if (vel.x > maxVel) vel.x = maxVel;
	if (vel.y > maxVel) vel.y = maxVel;
	/*if (utils::length(vel) < minVel) {
		vel *= minVel / utils::length(vel);
	}*/

	angle = utils::degrees(atan2f(vel.y, vel.x));

	rocket.setPosition(position);
	rocket.setRotation(angle);
}