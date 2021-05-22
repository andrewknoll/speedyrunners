#include "LobbyInterface.h"
#include "SFML/Network.hpp"
#include "NetworkUtils.h"

sf::Socket::Status LobbyInterface::sendAndReceive(const OnlineRequest& req, sf::Packet& ans, SocketPtr socket) const
{
	sf::Socket::Status status;
	sf::Packet pack_req;
	pack_req << req;

	status = socket->send(pack_req);

	if (status != sf::Socket::Done) {
		return status;
	}
	status = socket->receive(ans);

	if (status != sf::Socket::Done) {
		return status;
	}
}

std::vector<particles::PSystem>& LobbyInterface::getParticleSystems(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_PS;

		sf::Packet pack_ans;
		std::vector<particles::PSystem> ans;
		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = vectorOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->particleSystems = ans;
			return ans;
		}
	}

	return Lobby::getParticleSystems();

}

const std::list<LobbyInterface::ItemPtr>& LobbyInterface::getItems(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_IT;

		sf::Packet pack_ans;
		std::list<ItemPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		listOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->items = ans;
			return ans;
		}
	}
	else {
		return Lobby::getItems();
	}
}

const std::shared_ptr<RoundVictory> LobbyInterface::getRV(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_RV;

		sf::Packet pack_ans;
		std::shared_ptr<RoundVictory> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = sharedPtrOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->rv = ans;
			return ans;
		}
	}
	return Lobby::getRV();

}

const Countdown& LobbyInterface::getCountdown(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CD;

		sf::Packet pack_ans;
		std::shared_ptr<Countdown> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = sharedPtrOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->countdown = *ans;
			return *ans;
		}
	}
	return Lobby::getCountdown();

}

Camera LobbyInterface::getCamera(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CAM;

		sf::Packet pack_ans;
		std::shared_ptr<Camera> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = sharedPtrOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->cam = *ans;
			return *ans;
		}
	}
	return Lobby::getCamera();
}

Level LobbyInterface::getLevel(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_LVL;

		sf::Packet pack_ans;
		std::shared_ptr<Level> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = sharedPtrOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->lvl = *ans;
			return *ans;
		}
	}
	return Lobby::getLevel();

}

const std::vector<Checkpoint>& LobbyInterface::getCheckpoints(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CHP;

		sf::Packet pack_ans;
		std::vector<Checkpoint> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = vectorOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->checkpoints = ans;
			return ans;
		}
	}
	return Lobby::getCheckpoints();
}

const std::vector<LobbyInterface::NPCPtr>& LobbyInterface::getNPCs(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_NPC;

		sf::Packet pack_ans;
		std::vector<NPCPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = vectorOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->npcs = ans;
			return ans;
		}
	}
	return Lobby::getNPCs();
}

const std::vector<LobbyInterface::CharPtr>& LobbyInterface::getCharacters(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CHAR;

		sf::Packet pack_ans;
		std::vector<CharPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans = vectorOverloadOut(pack_ans, ans);
		if (status == sf::Socket::Done) {
			this->characters = ans;
			return ans;
		}
	}
	return Lobby::getCharacters();
}

LobbyInterface::State LobbyInterface::getState(SocketPtr socket) {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_ST;

		sf::Packet pack_ans;
		int ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans >> ans;
		if (status == sf::Socket::Done) {
			this->state = (Lobby::State)ans;
			return (Lobby::State)ans;
		}
	}
	return Lobby::getState();
}

/*void LobbyInterface::loopMenu()
{
	
}*/

void LobbyInterface::setReady(std::string lobbyID, int id, bool r, SocketPtr socket) {
	if (onlineMode && socket) {
		OnlineRequest o;
		o.type = OnlineRequest::READY;
		o.stringParam = lobbyID;
		o.intParam = id;
		o.boolParam = r;
		sf::Packet pack;
		pack << o;
		socket->send(pack);
	}
	else {
		Lobby::requestSetReady(id, r);
	}
}

void LobbyInterface::requestChangeCharacter(std::string lobbyID, int id, bool npc, glb::characterIndex idx, SocketPtr socket) {
	if (onlineMode && socket) {
		OnlineRequest o;
		o.type = OnlineRequest::CHARACTER_CHANGE;
		o.stringParam = lobbyID;
		o.intParam = id;
		o.ciParam = idx;
		o.boolParam = npc;
		sf::Packet pack;
		pack << o;
		socket->send(pack);
	}
	else {
		Lobby::changeCharacter(id, npc, idx);
	}
}