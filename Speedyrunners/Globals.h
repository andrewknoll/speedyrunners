#pragma once
#include <map>
#include <SFML/Graphics.hpp>
class Globals
{
private:
	std::map<std::string, sf::IntRect> hitboxes;
public:
	Globals();
};

