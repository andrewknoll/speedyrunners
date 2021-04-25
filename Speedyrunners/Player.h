#pragma once
#include "Character.h"
#include "Globals.hpp"
#include "PlayerSlot.h"
class Player : public PlayerSlot
{
	using CharPtr = std::shared_ptr<Character>;
private:
	bool holdingX = false;
	bool holdingDown = false;
public:
	Player();
	bool captureEvents(const sf::Event& event);
};

