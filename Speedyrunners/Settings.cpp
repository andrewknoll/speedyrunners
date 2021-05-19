#include <fstream>
#include <iostream>
#include "Settings.h"


Settings::Settings()
{
	load();
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

void Settings::setControlScheme(const std::vector<sf::Keyboard::Key> &scheme, int id)
{
	std::cout << "in:\n";
	for (auto k : scheme) std::cout << k << " ";
	std::cout << "\nSetting scheme: " << id<< " \n";
	for (auto k : controlSchemes[id]) std::cout << k << " ";
	controlSchemes[id] = scheme;
	std::cout << "\nafter:\n";
	for (auto k : controlSchemes[id]) std::cout << k << " ";
}

std::string Settings::getLocalisationPath() const
{
	return localisationPath;
}

void Settings::save(const std::string& file) const
{
	std::ofstream f(file);
	for (const auto& scheme : controlSchemes) {
		for (const int k : scheme)
			if (!(f << k << " ")) std::cerr << "Error saving settings!\n";
		f << "\n";
	}
}

void Settings::load(const std::string& file)
{
	std::ifstream f(file);
	int code;
	for (auto& scheme : controlSchemes) {
		for (auto& k : scheme) {
			if (!(f >> code)) std::cerr << "Error loading settings!\n";;
			k = sf::Keyboard::Key(code);
		}
		f.ignore(); // \n
	}
}

void Settings::editControl(const int idControl, const Settings::Key key, const sf::Keyboard::Key newKey)
{
	controlSchemes[idControl][(int) key] = newKey;
}

std::string Settings::to_string(const sf::Keyboard::Key& k)
{
	int code = k;
	if (code >=0 && code <= 25) return std::string(1, char('A' + code)); // letter
	else if (code == 71) return "Left";
	else if (code == 72) return "Right";
	else if (code == 73) return "Up";
	else if (code == 74) return "Down";
	else if (k == sf::Keyboard::Space) return "Space";
	int Numpad9 = sf::Keyboard::Key::Numpad9;
	if (code >= 75 && code <= Numpad9) return "Numpad" + std::string(1, char('0' + code - 75));
	return ""; // anything else not allowed
}