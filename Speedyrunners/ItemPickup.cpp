#include "ItemPickup.h"
#include "Resources.h"

void ItemPickup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (available) target.draw(sprite); // Only draw if available
}

ItemPickup::ItemPickup(const sf::Vector2f& pos, float tileWidth)
	: tex(Resources::getInstance().getMiscTexture(5)), sprite(tex)
{
	sprite.setTextureRect(sf::IntRect(0, 0, tex.getSize().x / 10, tex.getSize().y / 2));
	float rel = 1.75 * tileWidth / sprite.getGlobalBounds().width;
	sprite.setScale(rel, rel);
	utils::centerOrigin(sprite);
	sprite.setPosition(pos);
	collidable = sprite.getGlobalBounds();
}

sf::Vector2f ItemPickup::getPosition() const
{
	return sprite.getPosition();
}

bool ItemPickup::isInside(const sf::FloatRect& hitbox) const
{
	if (available) {
		return collidable.intersects(hitbox);
	}
	else return false;
}

void ItemPickup::update(sf::Time dT)
{
	if (!available) {
		cdAvailable -= dT;
		if (cdAvailable < sf::seconds(0)) {
			available = true; // Respawn
		}
	}
}

glb::item ItemPickup::getItem()
{
	available = false;
	cdAvailable = glb::itemPickupRespawn;	// Disable until respawn
	return glb::item(rng::itemSampler.sample()); // and return the item
}
