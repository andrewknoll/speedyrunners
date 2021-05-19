#include <fstream>
#include "Settings.h"


Settings::Settings()
{
	load(glb::SETTINGS_PATH);
}

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

void Settings::save(const std::string& file) const
{
	std::ofstream f(glb::SETTINGS_PATH);
	for (const auto& scheme : controlSchemes) {
		for (const int k : scheme)
			f << k << " ";
		f << "\n";
	}
}

void Settings::load(const std::string& file)
{
	std::ifstream f(glb::SETTINGS_PATH);
	int code;
	for (auto& scheme : controlSchemes) {
		for (auto& k : scheme) {
			f >> code;
			k = sf::Keyboard::Key(code);
		}
		f.ignore(); // \n
	}
}

void Settings::editControl(const int idControl, const Settings::Key key, const sf::Keyboard::Key newKey)
{
	controlSchemes[idControl][(int) key] = newKey;
}
