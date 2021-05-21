#include "Player.h"


#define WEIRD_CHEATS

Player::Player(const Settings& settings, int id) {
	std::vector<sf::Keyboard::Key> controls;
	settings.getControlScheme(controls, id);
	int i = 0;
	k_left = controls[i++];
	k_right = controls[i++];
	k_down = controls[i++];
	k_jump = controls[i++];
	k_hook = controls[i++];
	k_item = controls[i++];
	k_boost = controls[i++];
	type = 0;
}