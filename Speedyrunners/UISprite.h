#pragma once
#include <vector>
#include "Settings.h"
#include "UIElement.h"
class UISprite : public UIElement//, public sf::Sprite
{
	sf::Texture texture;
	sf::Sprite sprite;

	std::vector<sf::FloatRect> relTextureRects;

	bool ignoreCamera = false;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setRelativeTextureRect(const sf::FloatRect& relRect);
	void setRelativeTextureRect(const int idx);

	void makeClickable();
public:
	UISprite(std::string texturePath);

	UISprite(std::string texturePath, const Settings& settings, const float& relativeSize, 
		const sf::Vector2f& relativePosition, bool clickable, const std::vector<sf::FloatRect>& relativeTextureRects=std::vector<sf::FloatRect>());

	

	void loadTexture(std::string texturePath);

	bool mouseInside(const sf::RenderWindow& window) override;

	// load Texture on top of the previous one
	//void loadTextureOnTop(std::string texturePath);

	void setPosition(const sf::Vector2f& pos);

	void setHead(const sf::Vector2f& pos, const float& scale, const sf::RenderWindow& window);

	void setRelativeScale(const float& scale, const sf::RenderWindow& window);

	void setIgnoreCamera(bool ignore);
	//void setHead(const sf::Vector2f& pos, const float& scale);

	//virtual void draw(sf::RenderWindow& window) const override;
};

