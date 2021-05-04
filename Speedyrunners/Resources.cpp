#include "Resources.h"
#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>

Resources::Resources() {
	std::ifstream file(PATH_TO_ASSETS + RESOURCES_CSV);

	if (!file.is_open()) {
		throw std::runtime_error("Resources file cannot be read. Make sure the file " + RESOURCES_CSV + " is not corrupt.");
	}

	std::string buffer, type, token[4];
	std::getline(file, buffer, '\n');

	int i;

	while (!file.eof()) {
		std::istringstream iss(buffer);
		std::getline(iss, type, ',');

		if (type == "S") {
			for(int i = 0; i < 3; i++) std::getline(iss, token[i], ',');
			i = stoi(token[0]);

			Spritesheet sp;
			sp.parse_spritesheet(PATH_TO_ASSETS + token[1], PATH_TO_ASSETS + token[2]);
			if (spritesheets.size() <= i) {
				spritesheets.push_back(std::vector<Spritesheet>());
			}
			spritesheets[i].push_back(sp);
		}
		else if (type == "M") {
			for (int i = 0; i < 2; i++) std::getline(iss, token[i], ',');
			musicPlayer.addTrack(PATH_TO_ASSETS + token[1], stoi(token[0]));
		}
		else if (type == "A") { // Audio (soundbuffers)
			for (int i = 0; i < 2; i++) std::getline(iss, token[i], ',');
			soundBuffers.emplace_back(); // add one
			if (!soundBuffers.back().loadFromFile(PATH_TO_PLAYER_SFX + token[1]))
				std::cerr << "could not load sound buffer at " << PATH_TO_PLAYER_SFX + token[1] << "\n";
		}
		/*else if (type == "S") {
			sounds[current[2]++].loadFromFile(token[0]);
		}*/
		else if (type == "T") {
			for (int i = 0; i < 2; i++) std::getline(iss, token[i], ',');
			otherTextures[stoi(token[0])].loadFromFile(PATH_TO_ASSETS + token[1]);
		}
		else if (type == "I") {
			for (int i = 0; i < 2; i++) std::getline(iss, token[i], ',');
			items[stoi(token[0])].loadFromFile(PATH_TO_ASSETS + token[1]);
		}
		/*else if (type == "L") {

		}*/
	std::getline(file, buffer, '\n');
	}
}

Resources& Resources::getInstance() {
	static Resources src_;
	return src_;
}

const Spritesheet& Resources::getSpriteSheet(int character, int variant) {
	return spritesheets[character][variant];
}

const sf::Texture& Resources::getItemTexture(glb::item type) {
	return items[type - 1];
}

const sf::Texture& Resources::getMiscTexture(int type) {
	return otherTextures[type];
}

const sf::SoundBuffer& Resources::getSoundBuffer(int type)
{
	return soundBuffers[type];
}

const std::vector<sf::SoundBuffer>& Resources::getSoundBuffers()
{
	return soundBuffers;
}



/**
AudioPlayer& Resources::getAudioPlayer()
{
	return audioPlayer;
}*/

/* No renta
const sf::FloatRect& Resources::getMiscTextureRect(int type) {
	return otherTexturesRect[type];
}*/