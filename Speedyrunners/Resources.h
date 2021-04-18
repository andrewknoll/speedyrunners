#pragma once
#include "Spritesheet.h"
#include "SFML/Audio.hpp"
#include "MusicPlayer.h"
class Resources
{
private:
	Resources();

	std::vector<std::vector<Spritesheet> > spritesheets;
	//std::vector<sf::Sound> sounds;
	std::vector<sf::Texture> items;

	const std::string PATH_TO_ASSETS = "../assets/";
	const std::string RESOURCES_CSV = "resources.csv";

public:
	Resources(Resources &other) = delete;
	void operator=(const Resources&) = delete;
	static Resources& getInstance();

	Spritesheet getSpriteSheet(int character, int variant = 0);
	sf::Texture getItemTexture(int type);

	MusicPlayer musicPlayer;
};

