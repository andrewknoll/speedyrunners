#include "Rocket.h"

Rocket::Rocket(Character& target) : 
	target(target),
	particles(3)
{
}

void Rocket::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(particles, states);

}

void Rocket::update(sf::Time elapsed) {
	//Move
	position += vel * elapsed.asSeconds();
	particles.setEmitter(position);

	//Update velocity and angle
	vel = vel + acc * elapsed.asSeconds();
	angle = atan2f(vel.y, vel.x);
}