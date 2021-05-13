#pragma once
#include <random>

#include "SFML/Graphics.hpp"
#include "Globals.hpp"
#include "utils.hpp"

class ItemPickup : public sf::Drawable {
	// Capsule thing in the levels that give you items
protected:

	const sf::Texture& tex;

	sf::Sprite sprite;

	sf::FloatRect collidable;

	bool available = true; 

	sf::Time cdAvailable = glb::itemPickupRespawn;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


public:

	ItemPickup(const sf::Vector2f& pos, float tileWidth);

	sf::Vector2f getPosition() const;

	bool isInside(const sf::FloatRect& hitbox) const;

	void update(sf::Time dT);

	glb::item getItem();

};

