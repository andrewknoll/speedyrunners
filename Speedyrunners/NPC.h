#pragma once
#include "Character.h"
#include "Player.h"
class NPC : public Player
{
public:
	virtual void captureEvents(const sf::Event& event);
};

