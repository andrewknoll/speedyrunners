#include "LobbyInterface.h"
#include "SFML/Network.hpp"

template <typename T> 
sf::Packet operator<< (const sf::Packet& p, const std::shared_ptr<T> obj) {
	p.append(*obj);
}

template <typename T>
sf::Packet operator>> (sf::Packet& p, std::shared_ptr<T> obj) {
	obj = std::shared_ptr<T>((T*)p.getData());
	return p;
}

template <typename T>
sf::Packet operator<< (sf::Packet& p, const std::vector<T> obj) {
	p << obj.size();
	for (T& t : obj) {
		p << t;
	}
	return p;
}

template <typename T>
sf::Packet operator>> (sf::Packet& p, std::vector<T>& obj) {
	sf::Uint32 size;
	auto content = p >> size;
	T* buffer = (T*)content.getData();
	obj = std::vector<T>(buffer, buffer + size);
	return p;
}

template <typename T>
sf::Packet operator<< (sf::Packet& p, const std::list<T> obj) {
	p << obj.size();
	for (T& t : obj) {
		p << t;
	}
	return p;
}

template <typename T>
sf::Packet operator>> (sf::Packet& p, std::list<T>& obj) {
	sf::Uint32 size;
	auto content = p >> size;
	T* buffer = (T*)content.getData();
	obj = std::list<T>(buffer, buffer + size);
	return p;
}

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

std::vector<particles::PSystem>& LobbyInterface::getParticleSystems(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_PS;

		sf::Packet pack_ans;
		std::vector<particles::PSystem> ans;
		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans >> ans;
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

		sf::Packet pack_ans;
		std::list<ItemPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans >> ans;
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Items");
		}
		return ans;
	}
	else {
		return Lobby::getItems();
	}
}

const std::shared_ptr<RoundVictory> LobbyInterface::getRV(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_RV;

		sf::Packet pack_ans;
		std::shared_ptr<RoundVictory> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans >> ans;
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

		sf::Packet pack_ans;
		std::shared_ptr<Countdown> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		pack_ans >> ans;
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve Countdown");
		}
		return *ans;
	}
	else {
		return Lobby::getCountdown();
	}
}

Camera LobbyInterface::getCamera(SocketPtr socket) const {
	if (onlineMode && socket) {

		OnlineRequest req;
		req.type = OnlineRequest::GET_CAM;

		sf::Packet pack_ans;
		Camera ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
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

		sf::Packet pack_ans;
		Level ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
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

		sf::Packet pack_ans;
		std::vector<Checkpoint> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
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

		sf::Packet pack_ans;
		std::vector<NPCPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
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

		sf::Packet pack_ans;
		std::vector<CharPtr> ans;

		sf::Socket::Status status = sendAndReceive(req, pack_ans, socket);
		if (status != sf::Socket::Done) {
			throw std::exception("Couldn't retrieve NPCs");
		}
		return ans;
	}
	else {
		return Lobby::getCharacters();
	}
}