#pragma once

#include "SFML/Graphics.hpp"
#include "Globals.hpp"

class AudioPlayer;

class Countdown 
{
	//sf::Time remaining;
	int currentSecond; 

	sf::Time period;

	std::string bgPath = glb::CONTENT_PATH + "UI/Countdown/Background.png";
	sf::Texture bg;
	sf::Sprite bgSprite;

	std::string textPath = glb::CONTENT_PATH + "UI/Countdown/Text.png";
	sf::Texture text;
	sf::Sprite textSprite;

	//int 
	//std::shared_ptr<sf::RenderWindow> window;

	//void updateSprite(const sf::RenderWindow& window);
	AudioPlayer& audioPlayer;


	void updateSprite();

	void playSecondSFX(const int second);

public:

	Countdown(const sf::RenderWindow& _window);

	void update(const sf::Time& dT);

	bool ended() const;

	void draw(sf::RenderWindow& window) const;

	void reset();

};

