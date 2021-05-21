#pragma once
#include "Player.h"
class LocalPlayer : public Player
{
public:
	LocalPlayer(const Settings& settings, int id = 0);

	virtual LocalPlayer::Event captureEvents(const sf::Event& event) override;
};

