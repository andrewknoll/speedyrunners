#pragma once
#include "Player.h"
#include "SFML/Network.hpp"
class OnlinePlayer : public Player
{
protected:
	sf::TcpSocket& socket;

public:
	OnlinePlayer(sf::TcpSocket& socket, const Settings& settings, int id = 0);

	virtual OnlinePlayer::Event captureEvents(const sf::Event& event) override;
};

