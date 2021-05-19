#pragma once
#include "Globals.hpp"
#include "SFML/Graphics.hpp"

const int N_CONTROLS = 7; // number of keys (left, right, down, jump, hook, item, boost)
class Settings
{
	using Controls = std::array<sf::Keyboard::Key, N_CONTROLS>;

	std::string localisationPath = glb::CONTENT_PATH + "Localisation/english";

	sf::Vector2i resolution;

public:
	enum class Key {
		LEFT, RIGHT, DOWN, JUMP, HOOK, ITEM
	};
protected:
	std::vector<std::vector<sf::Keyboard::Key>> controlSchemes {
		{sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Down, sf::Keyboard::Key::Z, sf::Keyboard::Key::X, sf::Keyboard::Key::C, sf::Keyboard::Key::Space},
		{sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::S, sf::Keyboard::Key::V, sf::Keyboard::Key::B, sf::Keyboard::Key::N, sf::Keyboard::Key::Space},
		{sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Down, sf::Keyboard::Key::Numpad1, sf::Keyboard::Key::Numpad2, sf::Keyboard::Key::Numpad3, sf::Keyboard::Key::Numpad0}
	};
	

public:
	Settings();


	void setResolution(const sf::Vector2i& res);

	void setLocalisation(const std::string& loc);

	void getControlScheme(std::vector<sf::Keyboard::Key>& scheme, int id) const;
	void setControlScheme(const std::vector<sf::Keyboard::Key>& scheme, int id);

	sf::Vector2i windowResolution() const;

	std::string getLocalisationPath() const;

	void save(const std::string& file=glb::SETTINGS_PATH) const;

	void load(const std::string& file=glb::SETTINGS_PATH);

	void editControl(const int idControl, const Settings::Key key, const sf::Keyboard::Key newKey);

	std::string to_string(const sf::Keyboard::Key& k); // Returns "" if the key is not supported
};

