#pragma once
#include "Item.h"
#include "Animation.h"
class IceRay : public Item {
	using CharPtr = std::shared_ptr<Character>;
private:
	Animation sourceAnim;
	Animation beamAnim;
	sf::Texture beamTex;
	sf::Sprite source;
	sf::Sprite beam;
	CharPtr user;

	//particles::PSystem& particleSyst;

	std::vector<sf::IntRect> beamRects; // texture rects

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	IceRay(CharPtr user, bool facingRight);
	void setTexRect(bool first); // Sets the first or the second tex rect
	virtual bool update(sf::Time elapsed, const Level& lvl) override;

	virtual void doThingTo(std::shared_ptr<Character> c) override; // Explode, in this case

};


};

