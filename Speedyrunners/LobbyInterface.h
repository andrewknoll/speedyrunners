#pragma once
#include "Lobby.h"
#include "OnlineRequest.h"
#include "SFML/Network.hpp"
class LobbyInterface : public Lobby
{
	using CharPtr = std::shared_ptr<Character>;
	using PlayerPtr = std::shared_ptr<Player>;
	using NPCPtr = std::shared_ptr<NPC>;
	using PSPtr = std::shared_ptr<PlayerSlot>;
	using ItemPtr = std::shared_ptr<Item>;
	using SocketPtr = std::shared_ptr<sf::TcpSocket>;
	/*using ServerAnswer = std::variant < std::vector<particles::PSystem>,
		std::list<ItemPtr>,
		std::shared_ptr<RoundVictory>,
		Countdown,
		Camera,
		Level,
		std::vector<Checkpoint>,
		std::vector<NPCPtr>,
		std::vector<CharPtr>
	>;*/

	//LobbyInterface will have access to PRIVATE, PROTECTED and PUBLIC elements of Lobby (since LobbyInterface is declared as a friend)
	//GameServer will have access to PROTECTED and PUBLIC elements of Lobby
	friend class GameServer;
private:
	template<class T>
	sf::Socket::Status sendAndReceive(const OnlineRequest& req, T& ans, SocketPtr socket) const;
public:
	std::vector<particles::PSystem>& getParticleSystems(SocketPtr = nullptr) const;
	const std::list<ItemPtr>& getItems(SocketPtr = nullptr) const;
	const std::shared_ptr<RoundVictory>& getRV(SocketPtr = nullptr) const;
	const Countdown& getCountdown(SocketPtr = nullptr) const;
	Camera getCamera(SocketPtr = nullptr) const;
	Level getLevel(SocketPtr = nullptr) const;
	const std::vector<Checkpoint>& getCheckpoints(SocketPtr = nullptr) const;
	const std::vector<NPCPtr>& getNPCs(SocketPtr = nullptr) const;
	const std::vector<CharPtr>& getCharacters(SocketPtr = nullptr) const;
};

