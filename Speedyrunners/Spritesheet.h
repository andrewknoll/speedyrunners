#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <map>
#include <iostream>
#include <fstream>
#include <ctype.h>
class Spritesheet
{
private:
	sf::Texture spritesheet;
	std::map<std::string, Animation> animations;
	sf::Sprite sprite;
	bool flags[13];

	bool remove_commas(std::string matched);
public:
	int process(std::string matched);
	int parse_spritesheet(std::string image_filename, std::string data_filename);
};

