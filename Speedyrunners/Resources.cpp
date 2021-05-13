#include "Resources.h"
#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>

Resources::Resources() :
	rocketsPartSystem() 
{
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
		else if (type == "WS") {
			for (int i = 0; i < 3; i++) std::getline(iss, token[i], ',');
			i = stoi(token[0]);

			Spritesheet sp;
			sp.setVictoryPose(true);
			sp.parse_spritesheet(PATH_TO_ASSETS + token[1], PATH_TO_ASSETS + token[2]);
			if (win_spritesheets.size() <= i) {
				win_spritesheets.push_back(std::vector<Spritesheet>());
			}
			win_spritesheets[i].push_back(sp);
		}
		else if (type == "M") {
			for (int i = 0; i < 2; i++) std::getline(iss, token[i], ',');
			musicPlayer.addTrack(PATH_TO_ASSETS + token[1], stoi(token[0]));
		}
		else if (type == "A" || type == "A2" || type == "A3" || type == "AW") { // Audio (soundbuffers)
			std::string path = PATH_TO_PLAYER_SFX;
			if (type == "A2") { // Ingame ui audio
				path = PATH_TO_INGAME_UI_SFX;
			}
			if (type == "AW") // Weapons
				path = PATH_TO_ITEMS_SFX;
			std::string line;
			std::getline(iss, line); // until \n
			std::istringstream liness(line);
			soundBuffers.emplace_back(); // new vector for a type of sound (eg jump)
			auto& bufs = soundBuffers.back(); // ref to the vector
			while (std::getline(liness, token[0], ',')) {// each , separated field
				bufs.emplace_back(); // new soundBuffer of that type
				if (!bufs.back().loadFromFile(path + token[0]))
					std::cerr << "could not load sound buffer at " << path + token[0] << "\n";
			}
			/*for (int i = 0; i < 2; i++) std::getline(iss, token[i], ',');
			soundBuffers.emplace_back(); // add a vector
			if (!soundBuffers.back().loadFromFile(PATH_TO_PLAYER_SFX + token[1]))
				std::cerr << "could not load sound buffer at " << PATH_TO_PLAYER_SFX + token[1] << "\n";*/
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
	//std::cout << 

	audioPlayer.loadSoundsFromBuffers(soundBuffers);
	rocketsPartSystem.setTexture(getMiscTexture(8), sf::FloatRect(0,0,0.5,1)); // only use first horizontal half
}

Resources& Resources::getInstance() {
	static Resources src_;
	return src_;
}

const Spritesheet& Resources::getSpriteSheet(int character, int variant) {
	return spritesheets[character][variant];
}

const Spritesheet & Resources::getVictorySpriteSheet(int character, int variant) {
	std::cout << win_spritesheets.size() << " win spritesheets in resources " << win_spritesheets[character].size() << " of character " << character << "  variant: " << variant << "\n";
	return win_spritesheets[character][variant];
}

const sf::Texture& Resources::getItemTexture(glb::item type) {
	return items[type - 1];
}

const sf::Texture& Resources::getMiscTexture(int type) {
	return otherTextures[type];
}


const std::vector<sf::SoundBuffer>& Resources::getSoundBuffer(int type)
{
	return soundBuffers[type];
}

const std::vector<std::vector<sf::SoundBuffer>>& Resources::getSoundBuffers()
{
	return soundBuffers;
}




AudioPlayer& Resources::getAudioPlayer()
{
	return audioPlayer;
}

/* No renta
const sf::FloatRect& Resources::getMiscTextureRect(int type) {
	return otherTexturesRect[type];
}*/