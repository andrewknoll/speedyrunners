#pragma once
#include "UIElement.h"
class UISprite : public UIElement, public sf::Sprite
{
	sf::Texture texture;
	sf::Sprite sprite;
public:
	
	void loadTexture(std::string texturePath);

	virtual void draw(sf::RenderWindow& window) const override;
};

