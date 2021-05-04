#pragma once
#include "Spritesheet.h"
#include "SFML/Audio.hpp"
#include "MusicPlayer.h"
#include "Globals.hpp"
#include "AudioPlayer.h"
class Resources
{
private:
	Resources();

	AudioPlayer audioPlayer;

	std::vector<std::vector<Spritesheet> > spritesheets;
	std::vector<sf::SoundBuffer> soundBuffers;
	std::vector<sf::Texture> items = std::vector<sf::Texture>(glb::NUMBER_OF_ITEMS);
	std::vector<sf::Texture> otherTextures = std::vector<sf::Texture>(glb::NUMBER_OF_MISC_TEXTURES);

	//std::vector<sf::FloatRect> otherTexturesRect = std::vector<sf::FloatRect>(glb::NUMBER_OF_MISC_TEXTURES);

	const std::string PATH_TO_ASSETS = "../assets/";
	const std::string PATH_TO_PLAYER_SFX = "../assets/Content/Audio/Sfx/Player/";
	const std::string RESOURCES_CSV = "resources.csv";

public:
	Resources(Resources &other) = delete;
	void operator=(const Resources&) = delete;
	static Resources& getInstance();

	const Spritesheet& getSpriteSheet(int character, int variant = 0);
	const sf::Texture& getItemTexture(glb::item type);
	const sf::Texture & getMiscTexture(int type);

	const sf::SoundBuffer& getSoundBuffer(int type);
	const std::vector<sf::SoundBuffer>& getSoundBuffers();

	AudioPlayer& getAudioPlayer();


	MusicPlayer musicPlayer;
};

