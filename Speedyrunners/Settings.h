#pragma once
#include "Globals.hpp"
#include "SFML/Graphics.hpp"

class Settings
{
	std::string localisationPath = glb::CONTENT_PATH + "Localisation/english";

	sf::Vector2i resolution;

public:
	void setResolution(const sf::Vector2i& res);

	void setLocalisation(const std::string& loc);

	sf::Vector2i windowResolution() const;

	std::string getLocalisationPath() const;

};

