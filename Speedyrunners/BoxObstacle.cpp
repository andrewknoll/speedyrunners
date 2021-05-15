#include "BoxObstacle.h"
#include "Resources.h"



void BoxObstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (available || falling) target.draw(sprite); // Only draw if available or falling
}

BoxObstacle::BoxObstacle(const sf::Vector2f& pos, float tileWidth)
	: tex(Resources::getInstance().getMiscTexture(12)), sprite(tex), position(pos), rotSpeed(0)
{
	int nSprites = 20;
	int idx = 0;
	if (glb::enableRandomObstacles) rng::defaultGen.rand(0, nSprites);
	int width = tex.getSize().x / nSprites; // Width of the sprite in pixels
	sprite.setTextureRect(sf::IntRect(idx * width, 0, width, tex.getSize().y));
	float rel = 1.75 * tileWidth / sprite.getGlobalBounds().width;
	sprite.setScale(rel, rel);
	utils::centerOrigin(sprite);
	sprite.setPosition(pos);
	collidable = sprite.getGlobalBounds();
}

sf::Vector2f BoxObstacle::getPosition() const
{
	return sprite.getPosition();
}

bool BoxObstacle::isInside(const sf::FloatRect& hitbox) 
{
	if (available && collidable.intersects(hitbox)) {
		available = false;
		cdAvailable = glb::itemPickupRespawn;
		falling = true;
		rotSpeed = rng::defaultGen.rand(-50.0f, 50.0f);
		vel = sf::Vector2f(rng::defaultGen.rand(-40.0f, 40.0f), rng::defaultGen.rand(-40.0f, 40.0f));
		return true;
	}
	else return false;
}

void BoxObstacle::update(sf::Time dT)
{
	if (!available) {
		cdAvailable -= dT;
		if (cdAvailable < sf::seconds(0)) {
			available = true; // Respawn
			falling = false;
			sprite.setPosition(position);
			sprite.setRotation(0);
		}
		else if (cdAvailable > glb::itemPickupRespawn*0.94f) { // fall "animation"
			float dtSec = dT.asSeconds();
			sprite.rotate(rotSpeed * dtSec);
			sprite.move(vel * dtSec);
			//sprite.setColor(sprite.getColor().rgb)
			vel.y += physics::GRAVITY * dtSec;
		}
		else {
			falling = false;
		}
	}
}

