#include "LobbyInterface.h"
#include "SFML/Network.hpp"

template<class T>
sf::Socket::Status LobbyInterface::sendAndReceive(const OnlineRequest& req, T& ans, SocketPtr socket) const
{
	sf::Socket::Status status;
	sf::Packet pack_req;
	pack_req << req;

	status = socket->send(pack_req);

	if (status != sf::Socket::Done) {
		return status;
	}
	sf::Packet pack_ans;
	status = socket->receive(pack_ans);

	if (status != sf::Socket::Done) {
		return status;
	}

	pack_ans >> ans;

}

std::vector<particles::PSystem>& LobbyInterface::getParticleSystems(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_PS;

		std::vector<particles::PSystem> ans;
		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Particle System");
		}
		return ans;
	}
	else {
		return Lobby::getParticleSystems();
	}
}

const std::list<LobbyInterface::ItemPtr>& LobbyInterface::getItems(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_IT;

		std::list<ItemPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Items");
		}
		return ans;
	}
	else {
		return Lobby::getItems();
	}
}

const std::shared_ptr<RoundVictory>& LobbyInterface::getRV(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_RV;

		std::shared_ptr<RoundVictory> ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Victory Screen");
		}
		return ans;
	}
	else {
		return Lobby::getRV();
	}
}

const Countdown& LobbyInterface::getCountdown(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CD;

		Countdown ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Countdown");
		}
		return ans;
	}
	else {
		return Lobby::getCountdown();
	}
}

Camera LobbyInterface::getCamera(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CAM;

		Camera ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Camera");
		}
		return ans;
	}
	else {
		return Lobby::getCamera();
	}
}

Level LobbyInterface::getLevel(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_LVL;

		Level ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Camera");
		}
		return ans;
	}
	else {
		return Lobby::getLevel();
	}
}

const std::vector<Checkpoint>& LobbyInterface::getCheckpoints(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CHP;

		std::vector<Checkpoint> ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Checkpoints");
		}
		return ans;
	}
	else {
		return Lobby::getCheckpoints();
	}
}

const std::vector<LobbyInterface::NPCPtr>& LobbyInterface::getNPCs(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_NPC;

		std::vector<NPCPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve NPCs");
		}
		return ans;
	}
	else {
		return Lobby::getNPCs();
	}
}
const std::vector<LobbyInterface::CharPtr>& LobbyInterface::getCharacters(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CHAR;

		std::vector<CharPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve NPCs");
		}
		return ans;
	}
	else {
		return Lobby::getCharacters();
	}
}