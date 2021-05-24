#pragma once
#include "Character.h"
#include "Globals.hpp"
#include "PlayerSlot.h"
class Player : public PlayerSlot
{
	using CharPtr = std::shared_ptr<Character>;
private:
	bool holdingX = false;
	bool holdingUp = false;
	bool holdingDown = false;

	bool cheatsEnabled;


	sf::Keyboard::Key k_left, k_right, k_down, k_jump, k_hook, k_item, k_boost;
public:
	//Player(int id = 0);
	Player(const Settings& settings, int id = 0, bool cheats=false);
	bool captureEvents(const sf::Event& event);
	void setCheats(bool enable);
};

