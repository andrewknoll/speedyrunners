#pragma once
#include "UIElement.h"
class UISprite : public UIElement//, public sf::Sprite
{
	sf::Texture texture;
	sf::Sprite sprite;
	bool ignoreCamera = false;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	UISprite(std::string texturePath);
	
	void loadTexture(std::string texturePath);

	// load Texture on top of the previous one
	//void loadTextureOnTop(std::string texturePath);

	void setPosition(const sf::Vector2f& pos);

	void setHead(const sf::Vector2f& pos, const float& scale, const sf::RenderWindow& window);

	void setRelativeScale(const float& scale, const sf::RenderWindow& window);

	void setIgnoreCamera(bool ignore);
	//void setHead(const sf::Vector2f& pos, const float& scale);

	//virtual void draw(sf::RenderWindow& window) const override;
};

