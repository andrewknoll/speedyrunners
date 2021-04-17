#include "Settings.h"

void Settings::setResolution(const sf::Vector2i& res)
{
	resolution = res;
}


sf::Vector2i Settings::windowResolution() const
{
	return resolution;
}
void Settings::setLocalisation(const std::string& loc)
{
	localisationPath = loc;
}

std::string Settings::getLocalisationPath() const
{
	return localisationPath;
}
