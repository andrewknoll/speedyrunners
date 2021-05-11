#pragma once
#include "SFML/Graphics.hpp"

class BoostBox : public sf::Drawable {
protected:

	const sf::Texture& tex;

	sf::Sprite sprite;

	sf::FloatRect collidable;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


public:

	BoostBox(const sf::Vector2f& pos, float tileWidth);

	sf::Vector2f getPosition() const;

	bool isInside(const sf::Vector2f& pos) const;

};

