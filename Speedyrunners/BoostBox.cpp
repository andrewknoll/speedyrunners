#include "BoostBox.h"
#include "Resources.h"
#include "utils.hpp"
//#include "Animation.h"

void BoostBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}

BoostBox::BoostBox(const sf::Vector2f& pos, float tileWidth) 
	: tex(Resources::getInstance().getMiscTexture(glb::BOOST_SECTION_TEX)), sprite(tex),
	animation(sf::Vector2i(5,3),tex),
	width(tileWidth*10),
	pSystem(Resources::getInstance().getParticleSystem(glb::particleSystemIdx::BOOST))
{
	sprite = animation.get_first_frame();
	//sprite.setTextureRect(sf::IntRect(0,0,tex.getSize().x/5, tex.getSize().y / 3));
	float rel = 10 * tileWidth / sprite.getGlobalBounds().width;
	sprite.setScale(rel, rel);
	sprite.setOrigin(0,sprite.getLocalBounds().height);
	sprite.setPosition(pos);
	collidable = sprite.getGlobalBounds();
	center = pos + sf::Vector2f(collidable.width / 2.0f, -collidable.height / 2.0f);
}

sf::Vector2f BoostBox::getPosition() const
{
	return sprite.getPosition();
}

bool BoostBox::isInside(const sf::Vector2f& pos) const
{
	return collidable.contains(pos);
}

void BoostBox::emitParticles() {
	if (currentParticleCD-- < 0) {
		sf::Vector2f variation(rng::defaultGen.rand(-width / 4, width / 4), rng::defaultGen.rand(-width / 8, width / 8));
		pSystem.emit(center + variation);
		currentParticleCD = particleCD;
	}
}

void BoostBox::update(const sf::Time& dT)
{
	countdown -= dT;
	if (countdown <= sf::Time::Zero) {
		emitParticles();
		animation.advance_frame(sprite);
		countdown = glb::LVL_ANIMATION_PERIOD*2.0f;
	}
}
