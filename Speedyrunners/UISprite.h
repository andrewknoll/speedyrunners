#pragma once
#include "UIElement.h"
class UISprite : public UIElement//, public sf::Sprite
{
	sf::Texture texture;
	sf::Sprite sprite;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	UISprite(std::string texturePath);
	
	void loadTexture(std::string texturePath);

	//virtual void draw(sf::RenderWindow& window) const override;
};

