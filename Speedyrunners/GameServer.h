#pragma once
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include "Lobby.h"
class GameServer
{
private:
	sf::TcpListener listener;
	sf::Clock clock;
	std::unordered_map<std::string, std::shared_ptr<Lobby> > lobbies;

	int lastPort = glb::GAME_PORT_0;

public:
	std::shared_ptr<Lobby> createLobby();
	void loop();
	void updateAllGames(sf::Time dT);
};

