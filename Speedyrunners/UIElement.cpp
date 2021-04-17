#include <iostream>
#include "UIElement.h"
#include "utils.hpp"

UIElement::UIElement() : clickable(false)
{
}

UIElement::UIElement(const sf::FloatRect& _clickableBox) : 
	clickable(true), clickableBox(_clickableBox)
{
}

void UIElement::setClickableArea(const sf::FloatRect& _clickableBox)
{
	clickable = true;
	clickableBox = _clickableBox;
}

/*void UIElement::draw(sf::RenderWindow& window) const
{
	std::cout << "draw de UIELEMENT.......\n";
}*/

bool UIElement::mouseInside(const sf::RenderWindow& window)
{
	return clickable && clickableBox.contains(utils::mousePosition2f(window)); // sf::Mouse::getPosition()
}
