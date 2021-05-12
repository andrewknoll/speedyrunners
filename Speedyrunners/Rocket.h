#pragma once
#include "Item.h"
#include "ParticleSystem.h"
#include "Character.h"

class Rocket : public Item
{
	using CharPtr = std::shared_ptr<Character>;
private:
	ParticleSystem particles;
	sf::Sprite rocket;
	CharPtr target;

	sf::Vector2f vel, acc, position;
	float angle;

	const float velValIni = 128.0f;
	const float maxVel = 200.0f;
	const float minVel = 10.0f;
	const float accVal = 1.0f;

	const float detonationRadius = 50.0f; // it activates at this distance from the target
	const float explosionRadius = 200.0f; // the explosion radius
	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	

public:
	Rocket(sf::Vector2f pos, CharPtr target, bool facingRight);
	virtual bool update(sf::Time elapsed) override;

	virtual void doThingTo(std::shared_ptr<Character> c) override; // Explode, in this case

};

