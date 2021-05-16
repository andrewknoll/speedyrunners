#pragma once
#include "Item.h"

class TNT : public Item
{
protected:
	const sf::Texture* tex;

	sf::Sprite sprite;

	bool detonated = false, falling = false;

	float explosionMultiplier = 2.5;

	sf::Vector2f position;
	sf::Vector2f vel;

	const float explosionRadius = 200.0f; // the explosion radius

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	TNT(const sf::Vector2f& pos, float tileWidth);

	bool update(sf::Time elapsed, const Level& lvl) override; // Returns true if the item should die (rocket explodes)

	void doThingTo(std::shared_ptr<Character> c) override; // item, do your thing

	void changeState() override;
};

