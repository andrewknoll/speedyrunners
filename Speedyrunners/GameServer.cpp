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
		auto lobby = createLobby();
		lobby->playerJoin(std::make_shared<OnlinePlayer>(socket, 1));
		sf::Packet lobbyInfo;
		lobbyInfo << lobby->getNextId() << lobby->getCode();
		socket.send(lobbyInfo);
	}
	else if (o.type == OnlineRequest::RANDOM) {
		for (auto& l : lobbies) {
			if (l.second->getState() == LobbyInterface::State::AcceptingPlayers) {
				l.second->playerJoin(std::make_shared<OnlinePlayer>(socket));
				foundMatch = true;
				sf::Packet lobbyInfo;
				lobbyInfo << l.second->getNextId() << l.second->getCode();
				socket.send(lobbyInfo);
				break;
			}
		}
		if (!foundMatch) {
			auto lobby = createLobby();
			lobby->playerJoin(std::make_shared<OnlinePlayer>(socket));
			sf::Packet lobbyInfo;
			lobbyInfo << 0 << lobby->getCode();
			socket.send(lobbyInfo);
		}
	}
	else if(o.type == OnlineRequest::JOIN && o.stringParam.size() == 4 ){
		sf::Packet lobbyInfo;
		if (lobbies[o.stringParam]->getState() == Lobby::State::AcceptingPlayers) {
			lobbies[o.stringParam]->playerJoin(std::make_shared<OnlinePlayer>(socket));
			lobbyInfo << lobbies[o.stringParam]->getNextId() << o.stringParam;
		}
		else {
			lobbyInfo << -1 << -1;
		}
		socket.send(lobbyInfo);
	}
	else if (o.type == OnlineRequest::READY  && o.stringParam.size() == 4) {
		lobbies[o.stringParam]->requestSetReady(o.intParam, o.boolParam);
	}
	else if (o.type == OnlineRequest::CHARACTER_CHANGE  && o.stringParam.size() == 4) {
		lobbies[o.stringParam]->changeCharacter(o.intParam, o.boolParam, o.ciParam);
	}
}