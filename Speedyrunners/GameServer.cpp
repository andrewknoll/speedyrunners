#include "GameServer.h"
#include "Globals.hpp"
#include "OnlinePlayer.h"
#include "OnlineRequest.h"
#include "utils.hpp"

std::shared_ptr<LobbyInterface> GameServer::createLobby() {
	int key[5];
	std::string keyString = "";
	rng::defaultGen.fillRand(key, glb::HASH_LENGTH, glb::ALPHANUMERIC_CHARACTERS[1].front(), glb::ALPHANUMERIC_CHARACTERS[1].back());
	for (int i : key) {
		keyString += (char)i;
	}
	std::shared_ptr<LobbyInterface> newLobby = std::make_shared<LobbyInterface>();
	lobbies[keyString] = newLobby;
	return newLobby;
}

void GameServer::loop() {
	listener.listen(glb::MATCHMAKING_PORT);
	
	bool foundMatch = false;
	sf::TcpSocket socket;
	listener.accept(socket);

	sf::Packet received;
	socket.receive(received);

	OnlineRequest o;
	received >> o;

	if (o.type == OnlineRequest::CREATE) {
		createLobby()->playerJoin(std::make_shared<OnlinePlayer>(socket, 1));
	}
	else if (o.type == OnlineRequest::RANDOM) {
		for (auto& l : lobbies) {
			if (l.second->getState() == LobbyInterface::State::AcceptingPlayers) {
				l.second->playerJoin(std::make_shared<OnlinePlayer>(socket));
				foundMatch = true;
				break;
			}
		}
		if (!foundMatch) {
			createLobby()->playerJoin(std::make_shared<OnlinePlayer>(socket));
		}
	}
	else if(o.stringParam.size() == 4){
		lobbies[o.stringParam]->playerJoin(std::make_shared<OnlinePlayer>(socket));
	}
	
}