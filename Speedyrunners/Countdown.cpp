#include <iostream>

#include "Countdown.h"
#include "Resources.h"

Countdown::Countdown(const sf::RenderWindow& _window)
	: currentSecond(3),
	period(sf::seconds(1)),
	audioPlayer(Resources::getInstance().getAudioPlayer())//, window(std::make_shared<sf::RenderWindow>(_window))
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

void Countdown::playSecondSFX(const int second) {
	audioPlayer.play(AudioPlayer::Effect::COUNTDOWN_SLIDE_IN);
	if (second>1)
		audioPlayer.play(AudioPlayer::Effect::COUNTDOWN_TIMER_3_2);
	else if (second == 1)
		audioPlayer.play(AudioPlayer::Effect::COUNTDOWN_TIMER_1);
	else // GO
		audioPlayer.play(AudioPlayer::Effect::COUNTDOWN_TIMER_GO);
}

void Countdown::update(const sf::Time& dT)
{
	//SOUND
	if (period == sf::seconds(1) && currentSecond == 3) {// first time
		audioPlayer.play(AudioPlayer::Effect::COUNTDOWN_TIMER_BUILDUP); 
		playSecondSFX(currentSecond);
	}
		
	period -= dT;
	if (period.asSeconds() < 0) {
		currentSecond--;
		playSecondSFX(currentSecond);
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
