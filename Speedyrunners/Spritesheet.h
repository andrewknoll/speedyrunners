#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <map>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <memory>
class Spritesheet
{
private:
	std::map<std::string, Animation> animations;
	bool flags[15];

	bool remove_commas_or_spaces(std::string& matched);
	int process_token(std::string matched);

	std::shared_ptr<sf::Texture> texture;
public:
	int parse_spritesheet(std::string image_filename, std::string data_filename);
	std::map<std::string, Animation> get_animations();
};

