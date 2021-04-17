#include <iostream>
#include "UIElement.h"

UIElement::UIElement() : clickable(false)
{
}

UIElement::UIElement(const sf::IntRect& _clickableBox) : 
	clickable(true), clickableBox(_clickableBox)
{
}

void UIElement::setClickableArea(const sf::IntRect& _clickableBox)
{
	clickable = true;
	clickableBox = _clickableBox;
}

/*void UIElement::draw(sf::RenderWindow& window) const
{
	std::cout << "draw de UIELEMENT.......\n";
}*/

bool UIElement::mouseInside() const
{
	return clickable && clickableBox.contains(sf::Mouse::getPosition());
}
