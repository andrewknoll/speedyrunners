#include "TNT.h"
#include "Resources.h"
#include "utils.hpp"
#include "Level.h"
#include "Character.h"

void TNT::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}

TNT::TNT(const sf::Vector2f& pos, float tileWidth) : 
	Item(glb::TNT), tex(&Resources::getInstance().getMiscTexture(glb::BOMB_TEX)), sprite(*tex), position(pos), falling(true),
	vel(rng::defaultGen.rand(-40.0f, 40.0f), rng::defaultGen.rand(-55.0f, 40.0f))
{
	sprite.setTextureRect(utils::relativeToGlobalTextureRect(sf::FloatRect(0,0.0,1,1.0/3.0), *tex));
	utils::setWidth(sprite, 2.125f * tileWidth);
	utils::centerOrigin(sprite);
	sprite.setPosition(pos);
}

int TNT::update(sf::Time elapsed, const Level& lvl)
{
	if (falling) {
		const auto& tiles = lvl.getCollidableTiles();
		if (!tiles.collision(sprite.getGlobalBounds()).empty()) { // collided with floor, stop
			audioPlayer.play(AudioPlayer::Effect::TNT_PLACE);
			falling = false;
		}
		else { // continue falling
			sprite.move(vel * elapsed.asSeconds());
			vel.y += physics::GRAVITY * elapsed.asSeconds();
		}
	}
	if (detonated) {// exploded
		audioPlayer.play(AudioPlayer::Effect::TNT_EXPLODE);
		Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_CLOUD).emit(sprite.getPosition());
		Resources::getInstance().getParticleSystem(glb::particleSystemIdx::SUDDEN_DEATH_EXPLOSION).emit(sprite.getPosition());
	}
	return detonated; // returns true if it should be removed
}

void TNT::doThingTo(std::shared_ptr<Character> c)
{
	auto dir = c->getPosition() - sprite.getPosition();
	auto dist = utils::length(dir);
	if (dist < explosionRadius) // in the explosion, add a vel based on distance to center
		c->getHitByTNT(explosionMultiplier * (explosionRadius-dist) * dir / dist);
}

void TNT::changeState()
{
	detonated = true;
}
