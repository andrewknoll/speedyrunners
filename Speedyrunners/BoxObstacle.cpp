#include "BoxObstacle.h"
#include "Resources.h"
#include "TileMap.h"



void BoxObstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (available || falling) target.draw(sprite); // Only draw if available or falling
#ifdef SHOW_BOX_HITBOX
	target.draw(hitboxShape);
#endif
}

BoxObstacle::BoxObstacle(const sf::Vector2f& pos, float tileWidth, bool respawn)
	: tex(&Resources::getInstance().getMiscTexture(12)), sprite(*tex), shouldRespawn(respawn), position(pos), rotSpeed(0)
{
	int nSprites = 20;
	int idx = 0;
	if (glb::enableRandomObstacles) idx = rng::defaultGen.rand(0, nSprites);
	int width = tex->getSize().x / nSprites; // Width of the sprite in pixels
	sprite.setTextureRect(sf::IntRect(idx * width, 0, width, tex->getSize().y));
	float rel = 1.75 * tileWidth / sprite.getGlobalBounds().width;
	sprite.setScale(rel, rel);
	utils::centerOrigin(sprite);
	sprite.setPosition(pos);
	collidable = sprite.getGlobalBounds();
}

void BoxObstacle::fallToFloor() {
	fallingToFloor = true;
	vel = sf::Vector2f(rng::defaultGen.rand(-25.0f, 25.0f), rng::defaultGen.rand(-55.0f, 40.0f));
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
		falling = true; fallingToFloor = false;
		rotSpeed = rng::defaultGen.rand(-50.0f, 50.0f);
		vel = sf::Vector2f(rng::defaultGen.rand(-40.0f, 40.0f), rng::defaultGen.rand(-40.0f, 40.0f));
		return true;
	}
	else return false;
}

void BoxObstacle::update(sf::Time dT, const TileMap& tiles)
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
			vel.y += physics::GRAVITY * dtSec;
		}
		else {
			falling = false;
		}
	}
	else if (fallingToFloor) {
		if (!tiles.collision(sprite.getGlobalBounds()).empty()) { // collided with floor, stop
			collidable = sprite.getGlobalBounds();
			fallingToFloor = false;
		}
		else { // continue falling
			sprite.move(vel * dT.asSeconds());
			vel.y += physics::GRAVITY * dT.asSeconds();
		}
	}
}

void BoxObstacle::setPositionCheckTiles(const sf::Vector2f& p, const TileMap& tiles) {
	setPosition(p);
#ifdef SHOW_BOX_HITBOX
	const auto& hb = sprite.getGlobalBounds();
	hitboxShape.setPosition(hb.left,hb.top);
	hitboxShape.setSize(sf::Vector2f(hb.width, hb.height));
	hitboxShape.setFillColor(sf::Color::Red);
#endif
	if (!tiles.collision(sprite.getGlobalBounds()).empty()) 
		std::cout << "colliding\n";
}

void BoxObstacle::setPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
}

bool BoxObstacle::respawnable() const
{
	return shouldRespawn;
}

