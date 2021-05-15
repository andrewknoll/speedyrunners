#include <cmath>
#include "Rocket.h"
#include "Resources.h"
#include "utils.hpp"
#include "Resources.h"
#include "Level.h"

Rocket::Rocket(sf::Vector2f pos, CharPtr target, bool facingRight, float width) :
	Item(glb::item::ROCKET),
	target(target),
	particleSyst(Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_SMOKE)),
	position(pos),
	audioPlayer(Resources::getInstance().getAudioPlayer())
{
	auto& t = Resources::getInstance().getItemTexture(glb::item::ROCKET);
	rocket.setTexture(t);

	rects.emplace_back(utils::relativeToGlobalTextureRect(sf::FloatRect(0, 0, 1, 0.5), t)); // Top half only
	rects.emplace_back(utils::relativeToGlobalTextureRect(sf::FloatRect(0, 0.5, 1, 0.5), t)); // Bottom half only
	
	setTexRect(true);
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

void Rocket::setTexRect(bool first) {
	rocket.setTextureRect(first ? rects[0] : rects[1]);
}

void Rocket::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	target.draw(rocket, states);
}

bool Rocket::update(sf::Time elapsed, const Level& lvl) { // todo: check tiles collisions
	sf::Vector2f diff;
	if (!lvl.getCollidableTiles().collision(rocket.getGlobalBounds()).empty()) { // Collision with tiles
		audioPlayer.play(AudioPlayer::Effect::ROCKET_EXPLODE);
		Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_CLOUD).emit(position);
		return true;
	}
	//Update Acceleration
	if (target != nullptr) {
		// Linear acc:
		diff = (target->getPosition() - position);
		float dist = utils::length(diff);
		//linearAcc = dist * accVal;
		//linearAcc = 8000;
		if (dist < detonationRadius) { // Detonate
			audioPlayer.play(AudioPlayer::Effect::ROCKET_EXPLODE);
			Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_CLOUD).emit(position);
			return true;
		}
		else if (dist < 2 * detonationRadius) { // Almost hit
			audioPlayer.setLoop(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP);
			audioPlayer.continuePlaying(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP);
		}
		else { // Stop the loop if it got away
			audioPlayer.setLoop(AudioPlayer::Effect::ROCKET_ALMOST_HIT_LOOP, false);
		}
		// angle:
		angle = atan2f(diff.y, diff.x); // in rad
	}
	// linear acc:
	sf::Vector2f facing(cos(angle), sin(angle));
	acc = facing * linearAcc;
	// opposed drag:
	sf::Vector2f up(-facing.x, facing.y);


	//Update velocity and angle
	vel = vel + acc * elapsed.asSeconds();
	// Apply drag:
	/*geometry::Mat2 base(facing, up);
	sf::Transform toWorld = base.toTransform();
	auto inverse = toWorld.getInverse();
	auto vLocal = inverse.transformPoint(vel);
	auto vertVel = std::abs(vLocal.x);
	//if (vertVel > 0.01f * minVel) { // if its too small, dont bother
		//vertVel -= horizontalDrag * elapsed.asSeconds();
	if (vertVel > maxVerticalVel && false) {
		vertVel = maxVerticalVel;
		vLocal.x = (vLocal.x < 0) ? -vertVel : vertVel;
		vel = toWorld.transformPoint(vLocal);
	}*/
	// reduce if maximum reached:
	float velMod = utils::length(vel);
	if (velMod > maxVel) vel *= maxVel / velMod; // make it maxVel
	else if (velMod < minVel) vel *= minVel / velMod; // make it minVel
	if (velMod > (maxVel + minVel) / 2.0f) setTexRect(false); // if more than half speed, blurry texture
	else setTexRect(true); // else normal

	//Move
	position += vel * elapsed.asSeconds();

	//angle = utils::degrees(atan2f(vel.y, vel.x));
	angle = utils::degrees(angle);
	rocket.setPosition(position);
	rocket.setRotation(angle);
	auto particlesPoint = rocket.getTransform().transformPoint(sf::Vector2f(-rocketLength, 10));
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




/*
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
			Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_CLOUD).emit(position);
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

	//Update velocity and angle
	vel = vel + acc * elapsed.asSeconds();
	float velMod = utils::length(vel);
	if (velMod > maxVel) vel *= maxVel / velMod; // make it maxVel
	else if (velMod < minVel) vel *= minVel / velMod; // make it minVel
	if (velMod > (maxVel + minVel) / 2.0f) setTexRect(false); // if more than half speed, blurry texture
	else setTexRect(true); // else normal

	//Move
	position += vel * elapsed.asSeconds();

	angle = utils::degrees(atan2f(vel.y, vel.x));

	rocket.setPosition(position);
	rocket.setRotation(angle);
	auto particlesPoint = rocket.getTransform().transformPoint(sf::Vector2f(-rocketLength, 0));
	particleSyst.emit(particlesPoint); // smoke!
	return false;
}
*/