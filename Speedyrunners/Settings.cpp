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

void Settings::getControlScheme(std::vector<sf::Keyboard::Key>& scheme, int id) const
{
	scheme = controlSchemes[id];
}

std::string Settings::getLocalisationPath() const
{
	return localisationPath;
}

void Settings::editControl(const int idControl, const Settings::Key key, const sf::Keyboard::Key newKey)
{
	controlSchemes[idControl][(int) key] = newKey;
}
