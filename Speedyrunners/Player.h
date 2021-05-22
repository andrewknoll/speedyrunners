#pragma once
#include "Character.h"
#include "Globals.hpp"
#include "PlayerSlot.h"
class Player : public PlayerSlot
{
	

	using CharPtr = std::shared_ptr<Character>;
protected:
	bool holdingX = false;
	bool holdingUp = false;
	bool holdingDown = false;

	sf::Keyboard::Key k_left, k_right, k_down, k_jump, k_hook, k_item, k_boost;

	Player(int id = 0);
	Player(const Settings& settings, int id = 0);

public:
	enum Event {
		NONE,
		USE_ITEM,
		DISCONNECTED
	};

	virtual Event captureEvents(const sf::Event& event) = 0;
};

