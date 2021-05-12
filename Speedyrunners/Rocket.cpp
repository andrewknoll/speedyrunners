#include "Rocket.h"
#include "Resources.h"
#include "utils.hpp"
#include <cmath>

Rocket::Rocket(sf::Vector2f pos, CharPtr target, bool facingRight) :
	Item(glb::item::ROCKET),
	target(target),
	particles(3),
	position(pos)
{
	auto& t = Resources::getInstance().getItemTexture(glb::item::ROCKET);
	rocket.setTexture(t);
	auto rect = utils::relativeToGlobalTextureRect(sf::FloatRect(0,0,1,0.5), t); // Top half only
	rocket.setTextureRect(rect);
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

bool Rocket::update(sf::Time elapsed) {
	sf::Vector2f diff;
	//Update Acceleration
	if (target != nullptr) {
		diff = (target->getPosition() - position);
		acc.x = accVal * (diff.x);
		acc.y = accVal * (diff.y);
		if (utils::length(diff) < detonationRadius) {
			return true;
		}
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
	return false;
}

void Rocket::doThingTo(std::shared_ptr<Character> c)
{
	float distance = utils::length(c->getPosition() - position);
	if (distance < explosionRadius) {
		c->getHitByRocket();
	}
}
