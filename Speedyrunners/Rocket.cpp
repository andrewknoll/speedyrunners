#include <cmath>
#include "Rocket.h"
#include "Resources.h"
#include "utils.hpp"
#include "Resources.h"

Rocket::Rocket(sf::Vector2f pos, CharPtr target, bool facingRight, float width) :
	Item(glb::item::ROCKET),
	target(target),
	particleSyst(Resources::getInstance().rocketsPartSystem),
	position(pos),
	audioPlayer(Resources::getInstance().getAudioPlayer())
{
	auto& t = Resources::getInstance().getItemTexture(glb::item::ROCKET);
	rocket.setTexture(t);
	auto rect = utils::relativeToGlobalTextureRect(sf::FloatRect(0,0,1,0.5), t); // Top half only
	rocket.setTextureRect(rect);
	utils::setWidth(rocket, width);
	utils::centerOrigin(rocket);
	if (facingRight) {
		vel = sf::Vector2f(velValIni, -velValIni);
	}
	else {
		vel = sf::Vector2f(-velValIni, -velValIni);
	}
	rocketLength = rocket.getGlobalBounds().width;

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::ROCKET_LAUNCH);
	audioPlayer.setLoop(AudioPlayer::Effect::ROCKET_FLY_LOOP);
	audioPlayer.continuePlaying(AudioPlayer::Effect::ROCKET_FLY_LOOP);

}

void Rocket::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	target.draw(rocket, states);
}

bool Rocket::update(sf::Time elapsed, const Level& lvl) { // todo: check tiles collisions
	sf::Vector2f diff;
	//Update Acceleration
	if (target != nullptr) {
		diff = (target->getPosition() - position);
		acc.x = accVal * (diff.x);
		acc.y = accVal * (diff.y);
		float dist = utils::length(diff);
		if (dist < detonationRadius) { // Detonate
			audioPlayer.play(AudioPlayer::Effect::ROCKET_EXPLODE);
			return true;
		}
		else if (dist < 2 * detonationRadius) { // Almost hit
			audioPlayer.setLoop(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP);
			audioPlayer.continuePlaying(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP);
		}
		else { // Stop the loop if it got away
			audioPlayer.setLoop(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP, false);
		}
	}

	//Move
	position += vel * elapsed.asSeconds();

	//Update velocity and angle
	vel = vel + acc * elapsed.asSeconds();
	float velMod = utils::length(vel);
	if (velMod > maxVel) vel *= maxVel / velMod; // make it maxVel
	else if (velMod < minVel) vel *= minVel / velMod; // make it minVel
	//if (vel.x > maxVel) vel.x = maxVel;
	//if (vel.y > maxVel) vel.y = maxVel;
	/*if (utils::length(vel) < minVel) {
		vel *= minVel / utils::length(vel);
	}*/

	angle = utils::degrees(atan2f(vel.y, vel.x));

	rocket.setPosition(position);
	rocket.setRotation(angle);
	auto particlesPoint = rocket.getTransform().transformPoint(sf::Vector2f(-rocketLength, 0));
	particleSyst.emit(particlesPoint); // smoke!
	return false;
}

void Rocket::doThingTo(std::shared_ptr<Character> c)
{
	float distance = utils::length(c->getPosition() - position);
	if (distance < explosionRadius) {
		c->getHitByRocket();
	}
	audioPlayer.stop(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP);
	audioPlayer.stop(AudioPlayer::Effect::ROCKET_FLY_LOOP);
}
