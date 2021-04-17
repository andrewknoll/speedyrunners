#include <iostream>

#include "Countdown.h"

Countdown::Countdown(const sf::RenderWindow& _window)
	: currentSecond(3),
	period(sf::seconds(1))//, window(std::make_shared<sf::RenderWindow>(_window))
{
	// Fondo:
	bg.loadFromFile(bgPath);
	bgSprite.setTexture(bg);
	float width = _window.getSize().x;
	bgSprite.setPosition(width/2.0f, 0);
	float relation = _window.getSize().y / bgSprite.getGlobalBounds().height;
	bgSprite.setScale(relation, relation);

	// Texto:
	text.loadFromFile(textPath);
	textSprite.setTexture(text);
	relation = _window.getSize().y / textSprite.getGlobalBounds().height / 2;
	textSprite.setScale(relation, relation);
	updateSprite();
	textSprite.setOrigin(textSprite.getLocalBounds().width / 2.0, textSprite.getLocalBounds().height / 2.0);
	textSprite.setPosition(width * 3.0 / 4.0, _window.getSize().y / 2.0);
}


void Countdown::updateSprite()
{
	sf::IntRect rect;
	rect.left = 0;
	rect.width = text.getSize().x;
	rect.height = text.getSize().y / 4;
	rect.top = rect.height * (3-currentSecond);
	textSprite.setTextureRect(rect);

	//textSprite.setOrigin(textSprite.getLocalBounds().width / 2.0, textSprite.getLocalBounds().height / 2.0);
	//textSprite.setPosition(width * 3.0 / 4.0, _window.getSize().y / 2.0);
}



void Countdown::update(const sf::Time& dT)
{
	period -= dT;
	if (period.asSeconds() < 0) {
		currentSecond--;
		updateSprite();
		period = sf::seconds(1);
	}
}

bool Countdown::ended() const
{
	return currentSecond<=0;
}

void Countdown::draw(sf::RenderWindow& window) const
{
	window.draw(bgSprite);
	window.draw(textSprite);
}
