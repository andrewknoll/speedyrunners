#pragma once
#include "SFML/Graphics.hpp"

#include "Animation.h"
#include "ParticleSystem.h"
//class Animation;
#include "utils.hpp"

class BoostBox : public sf::Drawable {
protected:

	const sf::Texture& tex;

	sf::Sprite sprite;
	sf::Vector2f center;
	sf::FloatRect collidable;

	Animation animation;

	sf::Time countdown = sf::seconds(rng::defaultGen.rand(0.0f, 2.0f*glb::LVL_ANIMATION_PERIOD.asSeconds()));
	
	float width;
	particles::ParticleSystem& pSystem;

	int particleCD = 7, currentParticleCD = particleCD; // too many particles without this

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void emitParticles();

public:

	BoostBox(const sf::Vector2f& pos, float tileWidth);

	sf::Vector2f getPosition() const;

	bool isInside(const sf::Vector2f& pos) const;

	

	void update(const sf::Time& dT);

};

