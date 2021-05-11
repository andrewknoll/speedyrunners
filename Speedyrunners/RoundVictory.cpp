#include "RoundVictory.h"
#include "Resources.h"
#include "utils.hpp"

RoundVictory::RoundVictory(const sf::RenderWindow& _window, const int characterIdx, const int characterVariant, const int characterScore) :
	currentSecond(4),
	t(sf::seconds(1)),
	audioPlayer(Resources::getInstance().getAudioPlayer()),
	characterScore(characterScore) 
{
	//TO-DO

	// Fondo:
	bg.loadFromFile(bgPath);
	bgSprite.setTexture(bg);
	float width = _window.getDefaultView().getSize().x;
	float height = _window.getDefaultView().getSize().y;
	bgSprite.setPosition(0, height / 2.0f);
	float relation = height / bgSprite.getGlobalBounds().height;

	sf::IntRect rect;
	rect.left = 0;
	rect.width = bg.getSize().x;
	rect.height = bg.getSize().y / 2;
	rect.top = 0;
	bgSprite.setTextureRect(rect);

	utils::scaleToFullScreenRatio(bgSprite, _window, 0.4);
	//bgSprite.setScale(relation, relation);

	characterVictoryPose = Resources::getInstance().getVictorySpriteSheet(characterIdx, characterVariant).get_animations()[0];
	mySprite = characterVictoryPose->get_first_frame();
	relation = height / mySprite.getGlobalBounds().height;
	utils::scaleToFullScreenRatio(mySprite, _window, 0.3);
	mySprite.setPosition(width * 0.2, height * 0.89);
}

void RoundVictory::update(const sf::Time& dT) {
	t -= dT;
	if (t <= sf::Time::Zero) {
		//SOUND
		if (currentSecond == 3) {
			if (characterScore == 1) {
				audioPlayer.play(AudioPlayer::Effect::WIN_ROUND_1);
			}
			else if (characterScore == 2) {
				audioPlayer.play(AudioPlayer::Effect::WIN_ROUND_2);
			}
		}
		currentSecond--;
		t = sf::seconds(1);
	}
	tickAnimation(dT);
}

void RoundVictory::tickAnimation(sf::Time dT) {
	animationCountdown -= dT;
	if (currentSecond < 3 && animationCountdown <= sf::Time::Zero) {
		characterVictoryPose->advance_frame(mySprite);
		animationCountdown = PERIOD;
	}
}

bool RoundVictory::ended() const {
	return currentSecond <= 0;
}

void RoundVictory::draw(sf::RenderWindow& window) const {
	//window.draw(bgSprite);
	//TO-DO A�adir n�mero de rondas seg�n el score
	if (currentSecond < 3) {
		auto view = window.getView();
		window.setView(window.getDefaultView());
		window.draw(bgSprite);
		window.draw(mySprite);
		window.setView(view);
		//window.draw(bgSprite);
		//window.draw(mySprite);
	}
}


