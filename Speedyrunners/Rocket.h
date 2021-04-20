#pragma once
#include "Item.h"
#include "ParticleSystem.h"
#include "Character.h"

class Rocket : public Item
{
private:
	ParticleSystem particles;
	sf::Sprite rocket;
	Character& target;

	sf::Vector2f acc, vel, position;
	float angle;
	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	

public:
	Rocket(Character& target);
	virtual void update(sf::Time elapsed);
};

