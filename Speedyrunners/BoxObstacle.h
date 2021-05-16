#pragma once
#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "Globals.hpp"
#include "utils.hpp"

//#define SHOW_BOX_HITBOX

class TileMap;

class BoxObstacle : public sf::Drawable {
	// Collidable obstacles
protected:

	const sf::Texture* tex;

	sf::Sprite sprite;

	sf::FloatRect collidable;

	bool available = true, falling = false, fallingToFloor = false;

	bool shouldRespawn = true;

	sf::Vector2f position;
	sf::Vector2f vel = sf::Vector2f(0, 0);
	float rotSpeed;


	sf::Time cdAvailable = glb::itemPickupRespawn;

#ifdef SHOW_BOX_HITBOX
	sf::RectangleShape hitboxShape;
#endif
	//Animation animation;
	//sf::Time countdown = sf::seconds(rng::defaultGen.rand(0.0f, glb::LVL_ANIMATION_PERIOD.asSeconds()));


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


public:

	BoxObstacle(const sf::Vector2f& pos, float tileWidth, bool respawn=true);

	void fallToFloor(); // makes the crate drop to the floor (remains collidable)

	sf::Vector2f getPosition() const;

	bool isInside(const sf::FloatRect& hitbox);

	void update(sf::Time dT, const TileMap& tiles);

	void setPositionCheckTiles(const sf::Vector2f& p, const TileMap& tiles);

	void setPosition(const sf::Vector2f& p);

	bool respawnable() const;
};
