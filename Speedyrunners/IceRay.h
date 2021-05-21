#pragma once
#include "Item.h"
#include "Animation.h"
#include "Character.h"
#include "Camera.h"
class IceRay : public Item {
	using CharPtr = std::shared_ptr<Character>;
	using AnimationPtr = std::shared_ptr<Animation>;
private:
	Animation sourceAnim;
	sf::Sprite source;
	particles::PSystem beamParticles;
	CharPtr user;

	const Camera& cam;
	bool facingRight, emitted = false;
	sf::Time countdown;
	sf::Time ttl;

	//particles::PSystem& particleSyst;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void tickAnimation(sf::Time dT);

public:
	IceRay(const Camera& cam, CharPtr user);
	//void setTexRect(bool first); // Sets the first or the second tex rect
	virtual int update(sf::Time elapsed, const Level& lvl) override;

	virtual void doThingTo(std::shared_ptr<Character> c) override; // Explode, in this case

};

