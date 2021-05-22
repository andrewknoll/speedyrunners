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
	sf::Socket::Status sendAndReceive(const OnlineRequest& req, sf::Packet& ans, SocketPtr socket) const;
public:
	std::vector<particles::PSystem>& getParticleSystems(SocketPtr socket = nullptr);
	const std::list<ItemPtr>& getItems(SocketPtr socket = nullptr);
	const std::shared_ptr<RoundVictory> getRV(SocketPtr socket = nullptr);
	const Countdown& getCountdown(SocketPtr socket = nullptr);
	Camera getCamera(SocketPtr socket = nullptr);
	Level getLevel(SocketPtr socket = nullptr);
	const std::vector<Checkpoint>& getCheckpoints(SocketPtr socket = nullptr);
	const std::vector<NPCPtr>& getNPCs(SocketPtr socket = nullptr);
	const std::vector<CharPtr>& getCharacters(SocketPtr socket = nullptr);
	State getState(SocketPtr socket = nullptr);
};

