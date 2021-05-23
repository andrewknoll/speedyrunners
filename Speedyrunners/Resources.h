#pragma once
#include "Spritesheet.h"
#include "SFML/Audio.hpp"
#include "MusicPlayer.h"
#include "Globals.hpp"
#include "AudioPlayer.h"
#include "ParticleSystem.h"
#include <variant>
class Resources
{
	using Other = std::variant<sf::IntRect>;
private:
	Resources();

	AudioPlayer audioPlayer;

	std::vector<std::vector<Spritesheet> > spritesheets;
	std::vector<std::vector<Spritesheet> > win_spritesheets;
	std::vector<Spritesheet> misc_spritesheets;
	std::vector<std::vector<sf::SoundBuffer>> soundBuffers;
	std::vector<sf::Texture> items = std::vector<sf::Texture>(glb::NUMBER_OF_ITEMS);
	std::vector<sf::Texture> otherTextures;
	std::vector<Other> otherResources;

	std::vector<particles::PSystem> particleSystems;// rocketsPartSystem;

	//std::vector<sf::FloatRect> otherTexturesRect = std::vector<sf::FloatRect>(glb::NUMBER_OF_MISC_TEXTURES);

	const std::string PATH_TO_ASSETS = "./assets/",
		PATH_TO_PLAYER_SFX = "./assets/Content/Audio/Sfx/Player/",
		PATH_TO_INGAME_UI_SFX = "./assets/Content/Audio/Sfx/UI/Ingame/",
		PATH_TO_ITEMS_SFX = "./assets/Content/Audio/Sfx/Weapons/",
		PATH_TO_MENU_SFX = "./assets/Content/Audio/Sfx/UI/Menu/";
	const std::string RESOURCES_CSV = "resources.csv";

public:
	Resources(Resources &other) = delete;
	void operator=(const Resources&) = delete;
	static Resources& getInstance();

	const Spritesheet& getSpriteSheet(int character, int variant = 0);
	const Spritesheet& getVictorySpriteSheet(int character, int variant = 0);
	const Spritesheet& getMiscSpriteSheet(int index);
	const sf::Texture& getItemTexture(glb::item type);
	const sf::Texture& getMiscTexture(int type);
	const Other& getOtherResources(int pos);

	const std::vector<sf::SoundBuffer>& getSoundBuffer(int type);
	const std::vector<std::vector<sf::SoundBuffer>>& getSoundBuffers();

	particles::PSystem& getParticleSystem(int idx);

	std::vector<particles::PSystem>& getParticleSystems();

	AudioPlayer& getAudioPlayer();


	MusicPlayer musicPlayer;

};

