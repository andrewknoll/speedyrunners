#include "TextElement.h"

TextElement::TextElement(const Settings& settings, const std::string& fontPath, const std::string& strText, 
	const float& relativeSize, const sf::Vector2f& relativePosition) 
{
	font.loadFromFile(fontPath);

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString(strText);

	// set the character size
	auto resolution = settings.windowResolution();
	text.setCharacterSize(relativeSize * resolution.y); // in pixels, not points!
	text.setPosition(resolution.x * relativePosition.x, resolution.y * relativePosition.y);


	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

void TextElement::setPosition(const sf::Vector2f& pos)
{
	text.setPosition(pos);
}

void TextElement::draw(sf::RenderWindow& window) const
{
	window.draw(text);
}
