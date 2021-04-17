#pragma once
#include "UIElement.h"
#include "Settings.h"
class TextElement : public UIElement
{
	sf::Font font;
	sf::Text text;

public:

	TextElement(const Settings& settings, const std::string& fontPath, const std::string& strText, 
		const float& relativeSize, const sf::Vector2f& relativePosition);


	void setPosition(const sf::Vector2f& pos);

	void draw(sf::RenderWindow& window) const;
};

