#include <cmath>
#include "IceRay.h"
#include "Resources.h"
#include "utils.hpp"
#include "Resources.h"
#include "Level.h"

IceRay::IceRay(CharPtr user, bool facingRight) :
	Item(glb::item::ICERAY),
	user(user)
{
	auto& anim = Resources::getInstance().getMiscSpriteSheet(0);
	auto& t = Resources::getInstance().getItemTexture(glb::item::ICERAY);

	beamAnim = Animation(1, 11, t);

	//setTexRect(true);

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::ICE_BEAM_FULL);

}

void IceRay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	target.draw(source, states);
	//TO-DO: Muchos beams
	target.draw(beam, states);
}
/*
bool IceRay::update(sf::Time elapsed, const Level& lvl) { // todo: check tiles collisions
	sf::Vector2f diff;
	if (!lvl.getCollidableTiles().collision(IceRay.getGlobalBounds()).empty()) { // Collision with tiles
		audioPlayer.play(AudioPlayer::Effect::IceRay_EXPLODE);
		Resources::getInstance().getParticleSystem(glb::particleSystemIdx::IceRay_CLOUD).emit(position);
		return true;
	}
	//Update Acceleration
	if (target != nullptr) {
		// Linear acc:
		diff = (target->getPosition() - position);
		float dist = utils::length(diff);
		if (dist < detonationRadius) { // Detonate
			audioPlayer.play(AudioPlayer::Effect::IceRay_EXPLODE);
			Resources::getInstance().getParticleSystem(glb::particleSystemIdx::IceRay_CLOUD).emit(position);
			return true;
		}
		else if (dist < 2 * detonationRadius) { // Almost hit
			audioPlayer.setLoop(AudioPlayer::Effect::IceRay_ALMOST_HIT_LOOP);
			audioPlayer.continuePlaying(AudioPlayer::Effect::IceRay_ALMOST_HIT_LOOP);
		}
		else { // Stop the loop if it got away
			audioPlayer.setLoop(AudioPlayer::Effect::IceRay_ALMOST_HIT_LOOP, false);
		}
		// angle:
		angle = atan2f(diff.y, diff.x); // in rad
	}
	// linear acc:
	sf::Vector2f facing(cos(angle), sin(angle));
	acc = facing * linearAcc;

	//Update velocity and angle
	vel = vel + acc * elapsed.asSeconds();
	// reduce if maximum reached:
	float velMod = utils::length(vel);
	if (velMod > maxVel) vel *= maxVel / velMod; // make it maxVel
	else if (velMod < minVel) vel *= minVel / velMod; // make it minVel
	if (velMod > (maxVel + minVel) / 2.0f) setTexRect(false); // if more than half speed, blurry texture
	else setTexRect(true); // else normal

	//Move
	position += vel * elapsed.asSeconds();

	angle = utils::degrees(angle);
	IceRay.setPosition(position);
	IceRay.setRotation(angle);
	auto particlesPoint = IceRay.getTransform().transformPoint(sf::Vector2f(-IceRayLength, 10));
	particleSyst.emit(particlesPoint); // smoke!
	return false;
}

void IceRay::doThingTo(std::shared_ptr<Character> c)
{
	float distance = utils::length(c->getPosition() - position);
	if (distance < explosionRadius) {
		c->getHitByIceRay();
	}
	audioPlayer.stop(AudioPlayer::Effect::IceRay_ALMOST_HIT_LOOP);
	audioPlayer.stop(AudioPlayer::Effect::IceRay_FLY_LOOP);
}
*/

