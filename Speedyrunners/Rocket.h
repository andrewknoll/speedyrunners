#pragma once
#include "Item.h"
#include "ParticleSystem.h"
#include "Character.h"

class Rocket : public Item
{
	using CharPtr = std::shared_ptr<Character>;
private:
	//ParticleSystem particles;
	sf::Sprite rocket;
	CharPtr target;

	sf::Vector2f vel, acc, position;

	float linearAcc = 4000;

	float angle = 0;

	AudioPlayer& audioPlayer;
	particles::ParticleSystem& particleSyst;

	std::vector<sf::IntRect> rects; // texture rects


	const float velValIni = 300.0f;
	const float maxVel = 500.0f; // modulo
	const float minVel = 150.0f;
	const float accVal = 1.0f;

	const float detonationRadius = 50.0f; // it activates at this distance from the target
	const float explosionRadius = 200.0f; // the explosion radius
	
	float rocketLength; // length of the sprite in world (for particles)

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	

public:
	Rocket(sf::Vector2f pos, CharPtr target, bool facingRight, float width = 35); // width is its world width
	void setTexRect(bool first); // Sets the first or the second tex rect
	virtual bool update(sf::Time elapsed, const Level& lvl) override;

	virtual void doThingTo(std::shared_ptr<Character> c) override; // Explode, in this case

};

