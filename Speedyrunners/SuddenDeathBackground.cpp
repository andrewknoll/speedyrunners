#include "SuddenDeathBackground.h"

void SuddenDeathBackground::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(coloredRect);
}

SuddenDeathBackground::SuddenDeathBackground(sf::FloatRect rect) : sf::View(rect) {
	coloredRect = sf::RectangleShape(sf::Vector2f(32, 32));
	coloredRect.setPosition(0, 0);
	coloredRect.setFillColor(sf::Color::Black);
}

void SuddenDeathBackground::update(sf::RenderTarget& target) {
	coloredRect.setSize(sf::Vector2f(target.getSize().x, target.getSize().y));
	coloredRect.setPosition(target.getDefaultView().getCenter() - target.getDefaultView().getSize());
	sf::Uint16 redValue;
	if (active) {
		redValue = coloredRect.getFillColor().r;
		if (increasing) {
			redValue++;
		}
		else {
			redValue--;
		}
		if (redValue > 254) {
			increasing = false;
		}
		else if (redValue < 1) {
			increasing = true;
		}
		coloredRect.setFillColor(sf::Color(redValue, 0, 0, 255));
	}
	else {
		coloredRect.setFillColor(sf::Color::Black);
	}
}

void SuddenDeathBackground::setActive(bool active) {
	this->active = active;
}

sf::RectangleShape SuddenDeathBackground::getRectangle() const {
	return coloredRect;
}
