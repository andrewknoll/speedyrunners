#pragma once
#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "Globals.hpp"
#include "utils.hpp"

class BoxObstacle : public sf::Drawable {
	// Collidable obstacles
protected:

	const sf::Texture& tex;

	sf::Sprite sprite;

	sf::FloatRect collidable;

	bool available = true, falling = false;

	sf::Vector2f position;
	sf::Vector2f vel = sf::Vector2f(0, 0);
	float rotSpeed;


	sf::Time cdAvailable = glb::itemPickupRespawn;

	//Animation animation;
	//sf::Time countdown = sf::seconds(rng::defaultGen.rand(0.0f, glb::LVL_ANIMATION_PERIOD.asSeconds()));


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


public:

	BoxObstacle(const sf::Vector2f& pos, float tileWidth);

	sf::Vector2f getPosition() const;

	bool isInside(const sf::FloatRect& hitbox);

	void update(sf::Time dT);

};
