#pragma once
#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "AudioPlayer.h"
#include "Globals.hpp"
class RoundVictory
{
private:
	//sf::Time remaining;
	int currentSecond;

	sf::Time t, animationCountdown;

	std::shared_ptr<Animation> characterVictoryPose;
	int characterScore;
	AudioPlayer& audioPlayer;
	const sf::Time PERIOD = sf::milliseconds(1000 / 30);
	sf::Sprite mySprite;

	std::string bgPath = glb::CONTENT_PATH + "UI/MultiplayerHUD/Bar.png";
	sf::Texture bg;
	sf::Sprite bgSprite;

public:
	RoundVictory(const sf::RenderWindow& _window, const int characterIdx, const int characterVariant, const int characterScore);
	void update(const sf::Time& dT);
	void tickAnimation(sf::Time dT);
	bool ended() const;
	void draw(sf::RenderWindow& window) const;
};

