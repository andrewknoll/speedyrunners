#include "BoostBox.h"
#include "Resources.h"

void BoostBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}

BoostBox::BoostBox(const sf::Vector2f& pos, float tileWidth) 
	: tex(Resources::getInstance().getMiscTexture(3)), sprite(tex)
{
	
	sprite.setTextureRect(sf::IntRect(0,0,tex.getSize().x/5, tex.getSize().y / 3));
	float rel = 10 * tileWidth / sprite.getGlobalBounds().width;
	sprite.setScale(rel, rel);
	sprite.setOrigin(0,sprite.getLocalBounds().height);
	sprite.setPosition(pos);
	collidable = sprite.getGlobalBounds();
}

sf::Vector2f BoostBox::getPosition() const
{
	return sprite.getPosition();
}

bool BoostBox::isInside(const sf::Vector2f& pos) const
{
	return collidable.contains(pos);
}
