#pragma once
#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "AudioPlayer.h"
#include "Globals.hpp"
class RoundVictory
{
private:

	float width = 1, height = 1;

	//sf::Time remaining;
	int currentSecond;

	sf::Time t, animationCountdown;

	std::shared_ptr<Animation> characterVictoryPose;
	int characterScore;
	AudioPlayer& audioPlayer;
	sf::Sprite mySprite;

	std::string bgPath = glb::CONTENT_PATH + "UI/MultiplayerHUD/Bar.png";
	sf::Texture bg;
	sf::Sprite bgSprite;

	std::vector<sf::Sprite> sprites;

public:
	RoundVictory(const int characterIdx, const int characterVariant, const int characterScore);
	void setWindow(const sf::RenderWindow& _window);
	void setRectForPoint(sf::Sprite& s, int points);
	void addScoreStuff(const sf::RenderWindow& _window, int score);
	void update(const sf::Time& dT);
	void tickAnimation(sf::Time dT);
	bool ended() const;
	void draw(sf::RenderWindow& window) const;
};

