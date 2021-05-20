#pragma once
#include "Item.h"
#include "ParticleSystem.h"
#include "Character.h"

class GoldenHook : public Item
{
	using CharPtr = std::shared_ptr<Character>;
private:
	//ParticleSystem particles;
	sf::Sprite hook;
	CharPtr from, target;

	sf::Vector2f vel, position, offset = glb::FEET_TO_HAND;

	bool flying = true, hooked = false, missed = false;

	sf::Time remainingPull = sf::seconds(0.75);

	float linearVel = 2000;
	float pullSpeed = 400;

	float angle = 0;

	particles::PSystem& particleSyst;

	std::vector<sf::IntRect> rects; // texture rects

	const float maxVertical = 0.5; // max vertical component of the direction (0.5 is 45º)

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setTexRect(bool first);


public:
	GoldenHook(CharPtr _from, CharPtr _target, float width=15); // width is its world width

	virtual bool update(sf::Time elapsed, const Level& lvl) override;

	virtual void doThingTo(std::shared_ptr<Character> c) override; // Explode, in this case

};

