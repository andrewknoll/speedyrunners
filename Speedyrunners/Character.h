#pragma once
#include "SFML/Graphics.hpp"

class Character : public sf::Sprite
{
	sf::Texture texture;
	std::string path;
public:
	Character();
	Character(const std::string& _path);
};

