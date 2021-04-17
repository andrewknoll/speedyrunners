#include "TextElement.h"
#include "utils.hpp"


TextElement::TextElement(const Settings& settings, const std::string& fontPath, const std::string& strText, 
	const float& relativeSize, const sf::Vector2f& relativePosition, bool clickable, const sf::Color& c) 
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
	text.setFillColor(c);

	if (clickable) makeClickable();

	// set the text style
	//text.setStyle(sf::Text::Bold);
}


void TextElement::setColor(const sf::Color& c)
{
	text.setFillColor(c);
}

void TextElement::setSecondColor(const sf::Color& c)
{
	secondColor = c;
}

void TextElement::swapColors()
{
	sf::Color aux = secondColor;
	secondColor = text.getFillColor();
	text.setFillColor(aux);
}



void TextElement::setPosition(const sf::Vector2f& pos)
{
	text.setPosition(pos);
}

void TextElement::makeClickable()
{
	setClickableArea(text.getGlobalBounds());
}

void TextElement::draw(sf::RenderWindow& window) const
{
	window.draw(text);
}

void TextElement::draw(sf::RenderWindow* window) const
{
	window->draw(text);
}

void TextElement::selectSecondColor() {
	if (firstColorSelected) {
		firstColorSelected = false;
		swapColors();
	} 
}

void TextElement::selectFirstColor() {
	if (!firstColorSelected) {
		firstColorSelected = true;
		swapColors();
	}
}

bool TextElement::mouseInside(const sf::RenderWindow& window)
{
	if (UIElement::mouseInside(window)) {
		selectSecondColor();
		return true;
	}
	else {
		selectFirstColor();
		return false;
	}
}
