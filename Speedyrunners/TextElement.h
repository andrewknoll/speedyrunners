#pragma once
#include "UIElement.h"
#include "Settings.h"
class TextElement : public UIElement
{
	sf::Font font;
	sf::Text text;
	sf::Color secondColor = sf::Color::White;
	bool firstColorSelected;

public:
	TextElement(const Settings& settings, const std::string& fontPath, const std::string& strText, 
		const float& relativeSize, const sf::Vector2f& relativePosition, 
		bool clickable = false, const sf::Color& c = sf::Color(51, 153, 255));

	void setColor(const sf::Color& c);

	void setSecondColor(const sf::Color& c = sf::Color::White);

	void swapColors();

	void makeClickable();

	void setPosition(const sf::Vector2f& pos);

	void draw(sf::RenderWindow& window) const override;
	void draw(sf::RenderWindow* window) const override;

	void selectSecondColor();

	void selectFirstColor();

	bool mouseInside(const sf::RenderWindow& window) override;
};

