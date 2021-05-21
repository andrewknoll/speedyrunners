#include "GameServer.h"
#include "Globals.hpp"
#include "OnlineRequest.h"
#include "utils.hpp"

std::shared_ptr<Lobby> GameServer::createLobby() {
	int key[5];
	std::string keyString = "";
	rng::defaultGen.fillRand(key, glb::HASH_LENGTH, glb::ALPHANUMERIC_CHARACTERS[1].front(), glb::ALPHANUMERIC_CHARACTERS[1].back());
	for (int i : key) {
		keyString += (char)i;
	}
	std::shared_ptr<Lobby> newLobby = std::make_shared<Lobby>();
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
		createLobby()->playerJoin(socket);
	}
	else if (o.type == OnlineRequest::RANDOM) {
		for (auto& l : lobbies) {
			if (l.second->getState() == Lobby::State::AcceptingPlayers) {
				l.second->playerJoin(socket);
				foundMatch = true;
				break;
			}
		}
		if (!foundMatch) {
			createLobby()->playerJoin(socket);
		}
	}
	else if(o.joinID.size() == 4){
		lobbies[o.joinID]->playerJoin(socket);
	}
	
}