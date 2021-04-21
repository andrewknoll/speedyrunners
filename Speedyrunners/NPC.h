#pragma once
#include "Character.h"
#include "Player.h"
class NPC : public Player
{
public:
	virtual bool captureEvents(const sf::Event& event);
};

