#include <iostream>
#include <time.h>

#include "Lobby.h"
#include "utils.hpp"
#include "Player.h"
#include "Menu.h"
#include "RoundVictory.h"

#include "Rocket.h"
#include "IceRay.h"
#include "GoldenHook.h"

#ifdef USE_IMGUI
#include "imgui.h"
#include "imgui-SFML.h"
#endif // USE_IMGUI
#include "TNT.h"


Lobby::Lobby()
	:
	state(State::Countdown),
	lvl(),
	cam(sf::FloatRect(0, 0, 1600, 900)),
	src(Resources::getInstance()),
	particleSystems(src.getParticleSystems())
	//dT(0)
{
	loadLevel("defaultLevel.csv");
}

void Lobby::update()
{
	int target;
	if (state != State::Editing) {
		cam.update(dT);
	}
	if (state == State::Playing && dT.asSeconds() < 0.1) {
		for (auto c : characters) {
			if (c->isDead()) continue;
			lvl.checkItemPickups(c); // check pickups, give the items to the character, etc
			lvl.checkBoxCollisions(c); // check boxes, collide with the player if needed
			c->update(dT, lvl);
		}
		lvl.update(dT); // respawn items, etc
		updateItems();
		for (auto &ps : particleSystems) ps.update(dT); // update particles
		updatePositions();
		cam.follow(characters);
		for (int i = 1; i < characters.size(); i++) {
			if (characters[i]->isDead()) continue;
			if (!cam.isInAllowedBounds(characters[i])) {
				//std::cout << "one down\n";
				//particleSystems[glb::SUDDEN_DEATH_EXPLOSION].burst(cam.closestInView(characters[i]->getPosition()),50,10); // TODO: no consigo sacar la posicion en el mundo de la camara
				characters[i]->die();
				src.getAudioPlayer().play(AudioPlayer::Effect::CHARACTER_OUTOFSCREEN);
				src.getAudioPlayer().play(AudioPlayer::Effect::DEATH);
				aliveCount--;
				suddenDeath = true;
				cam.setSuddenDeath(true);
			}
		}
		updateNPCs(true);
		if (characters.size() > 1 && aliveCount < 2) {
			cam.setSuddenDeath(false);
			state = State::FinishedRound;
			for (auto n : npcs) {
				n->clearPaths();
			}
			items.clear(); // delete all items
			clearParticles(); // and particles
			for (auto c : characters) {
				if (!c->isDead()) {
					c->increaseScore(1);
					if (c->getScore() >= 3) { // Won
						gameWon = true; // We dont finish here to let the anim play
					}
					//ui.updatePoints();
					break;
				}
			}
		}

	}
	else if (state == State::Editing && testingParticles && dT.asSeconds() < 0.1)
		for (auto& ps : particleSystems) ps.update(dT); // update particles
	else if (state == State::Countdown) {
		cam.follow(characters);
		cam.update(dT);
		updateNPCs(false);
		countdown.update(dT);
		if (countdown.ended()) {
			state = State::Playing;
		}
	}
	else if (state == State::FinishedRound) {
		cam.follow(characters);
		cam.update(dT);
		if (rv == nullptr) { // first time here, create round victory
			for (int i = 0; i < characters.size(); i++) {
				if (!characters[i]->isDead()) {
					clearParticles();
					rv = std::make_unique<RoundVictory>(characters[i]->getID(), characters[i]->getVariant(), characters[i]->getScore());
					respawnPosition = characters[i]->getLastSafePosition();
					rv->update(dT);
				}
				else {
					characters[i]->setAnimation(Character::AnimationIndex::StandAnim);
				}
			}
		}
		else {
			rv->update(dT);
			if (rv->ended()) {
				if (gameWon) {
					//loopMenu();
				}
				state = State::Countdown;
				countdown.reset();
				rv = nullptr;
				suddenDeath = false;
				aliveCount = characters.size();
				for (int i = 0; i < characters.size(); i++) {
					characters[i]->respawn(respawnPosition);
					bool canMoveSpawn = true;
					for (auto t : lvl.getCollidableTiles().tilesToTheSide(characters[i]->getHitBox(), false)) {
						canMoveSpawn &= t == Tiles::AIR;
					}
					if (canMoveSpawn) {
						respawnPosition.x -= 20;
					}
				}
				updatePositions();
				for (auto n : npcs) {
					n->setActiveCheckpoint(activeCheckpoint);
				}
				restartCv.notify_all();
			}
		}
	}
}

// update the positions based on distance to the active checkpoint
void Lobby::updatePositions()
{
	if (!checkpoints.empty()) {
		Checkpoint cp = checkpoints[activeCheckpoint];
		auto cpPos = cp.getPos();
		float d;
		for (auto c : characters) {
			if (c->isDead()) {
				d = INFINITY;
			}
			else {
				d = utils::distance(c->getPosition(), cpPos);
			}

			c->setDToCheckpoint(d);
			// get distance d to active checkpoint
			// if d < r: next checkpoint
		}
		// order characters and players based on distance
		std::sort(characters.begin(), characters.end(),
			[](auto & c1, auto & c2) {
			return c1->getDToCheckpoint() < c2->getDToCheckpoint();
		}
		);
		// Check if one has reached the checkpoint
		if (!characters.empty() && characters[0]->getDToCheckpoint() <= cp.getRadius()) {
			// Checkpoint reached, cycle to next
#ifdef VERBOSE_DEBUG
			std::cout << "Checkpoint " << activeCheckpoint << " reached\n";
#endif
			activeCheckpoint = (activeCheckpoint + 1) % checkpoints.size();
#ifdef VERBOSE_DEBUG
			std::cout << "(new = " << activeCheckpoint << ")\n";
#endif
		}
	}
}

void Lobby::handleItem(Lobby::ItemPtr item) {
	for (auto& c : characters) {
		item->doThingTo(c);
	}
}

void Lobby::clearParticles() {
	for (auto& ps : particleSystems) ps.clear();
}

void Lobby::updateItems() {
	int handle;
	// Weird loop to be able to erase (https://www.techiedelight.com/remove-elements-list-iterating-cpp/):
	auto itr = items.cbegin();
	while (itr != items.cend())
	{
		auto item = *itr;
		handle = item->update(dT, lvl);
		if (handle > 0) { // if the update returns 1 or 2, it should be handled
			handleItem(item); // handle
			if (handle == 1) {
				itr = items.erase(itr); // and delete
			}
		}
		if (handle != 1) {
			++itr;
		}
	}
}

void Lobby::updateNPCs(bool follow) {
	for (int i = 0; i < npcs.size(); i++) {
		if (npcs[i] != nullptr && checkpoints.size() > 0) {
			auto& followThread = threadPool[2 * i + 1];
			if (threadPool[2 * i].threadPtr == nullptr) {
				threadPool[2 * i].finished = false;
				threadPool[2 * i].threadPtr = std::make_unique<std::thread>([&, i]() {
					while (running) {
						if (npcs[i]->getCharacter()->isDead()) {
							npcs[i]->die();
							{ std::unique_lock<std::mutex> lck(restartMtx);
							restartCv.wait(lck); }
						}
						if (running) {
							npcs[i]->plan();
						}
					}
					threadPool[2 * i].finished = true;
					finishCV.notify_one();
				});
			}
			if (threadPool[2 * i + 1].threadPtr == nullptr) {
				threadPool[2 * i + 1].finished = false;
				threadPool[2 * i + 1].threadPtr = std::make_unique<std::thread>([&, i]() {
					while (running) {
						if (npcs[i]->getCharacter()->isDead()) {
							npcs[i]->die();
							{ std::unique_lock<std::mutex> lck(restartMtx);
							restartCv.wait(lck); }
						}
						if (running) {
							npcs[i]->plan();
						}
					}
					threadPool[2 * i + 1].finished = true;
					finishCV.notify_one();
				});
			}
			if (follow && state == State::Playing) {
				npcs[i]->update(dT);
			}
		}
	}
}

void Lobby::clear() {
	gameWon = false;
	//Resources::getInstance().getAudioPlayer().stopAll(); // stop all sfx
	characters.clear();
	players.clear();
	items.clear(); // delete all items
	clearParticles(); // and particles
	running = false;

	for (int i = 0; i < npcs.size(); i++) {
		npcs[i]->endMe();
	}

	std::unique_lock<std::mutex> lck(finishMtx);
	for (int i = 0; i < threadPool.size(); i++) {
		if (threadPool[i].threadPtr == nullptr) continue;
		restartCv.notify_all();	//Make sure the thread is not sleeping
		while (!threadPool[i].finished) {
			finishCV.wait_for(lck, std::chrono::seconds(2));
		}
		threadPool[i].threadPtr->join();
		threadPool[i].threadPtr = nullptr;
	}
	threadPool.clear();
	npcs.clear();

	countdown.end();
	cam.setSuddenDeath(false);


	threadPool.resize(8);
}

void Lobby::defaultInit(int N_PLAYERS, const Settings& settings) {

	sf::Vector2f spawnPosition = lvl.getInitialPosition();

	if (N_PLAYERS != 2) N_PLAYERS = 1;// only 1 or 2 players
	clear();

	std::shared_ptr<Character> speedyrunner = std::make_shared<Character>(src.getSpriteSheet(0), glb::SPEEDRUNNER);
	speedyrunner->setPosition(spawnPosition);

	bool canMoveSpawn = true;
	for (auto t : lvl.getCollidableTiles().tilesToTheSide(speedyrunner->getHitBox(), false)) {
		canMoveSpawn &= t == Tiles::AIR;
	}
	if (canMoveSpawn) {
		spawnPosition.x -= 20;
	}

	std::shared_ptr<Character> cosmonaut = std::make_shared<Character>(src.getSpriteSheet(1), glb::COSMONAUT);
	cosmonaut->setPosition(spawnPosition);

	canMoveSpawn = true;
	for (auto t : lvl.getCollidableTiles().tilesToTheSide(cosmonaut->getHitBox(), false)) {
		canMoveSpawn &= t == Tiles::AIR;
	}
	if (canMoveSpawn) {
		spawnPosition.x -= 20;
	}

	std::shared_ptr<Character> otro = std::make_shared<Character>(src.getSpriteSheet(2), glb::UNIC);
	otro->setPosition(spawnPosition);

	int id = 0;
	if (N_PLAYERS == 2) id = 1;
	std::shared_ptr<Player> me = std::make_shared<Player>(settings, id);
	me->setCharacter(speedyrunner);
	playerJoin(me);
	speedyrunner->setName("Player 1");
	addCharacter(speedyrunner);

	if (N_PLAYERS > 1) {
		std::shared_ptr<Player> secondPlayer = std::make_shared<Player>(settings, ++id);
		secondPlayer->setCharacter(otro);
		playerJoin(secondPlayer);
		otro->setName("Player 2");
		addCharacter(otro);
	}
	std::shared_ptr<NPC> other = std::make_shared<NPC>();
	other->setCharacter(cosmonaut);
	other->setCheckpoints(checkpoints);
	npcJoin(other);
	updatePositions();
	other->setActiveCheckpoint(activeCheckpoint);
	cosmonaut->setName("NPC");
	addCharacter(cosmonaut);
	cam.immediateFollow(characters);
	setState(State::Countdown);
	running = true;
}

void Lobby::defaultInit(const std::vector<glb::characterIndex>& _players, const std::vector<glb::characterIndex>& _npcs) {
	clear();
	int N_PLAYERS = players.size();
	sf::Vector2f spawnPosition = lvl.getInitialPosition();
	std::cout << "players: " << _players.size() << "\n";
	std::shared_ptr<Character> character;
	int i = 0;
	if (_players.size() == 2) i = 1; // controls
	for (auto c : _players) {
		std::cout << "adding player " << c << "\n";
		// character:
		character = std::make_shared<Character>(src.getSpriteSheet(c), c);
		character->setPosition(spawnPosition);
	/////	//  player:
	//	std::shared_ptr<Player> me = std::make_shared<Player>(getSettings(), i);
	//	me->setCharacter(character);
	//	playerJoin(me);
		character->setName("Player " + std::to_string((int)i));
		addCharacter(character);
		i++;
		bool canMoveSpawn = true;
		for (auto t : lvl.getCollidableTiles().tilesToTheSide(character->getHitBox(), false)) {
			canMoveSpawn &= t == Tiles::AIR;
		}
		if (canMoveSpawn) {
			spawnPosition.x -= 20;
		}
	}
	i = 0;
	for (auto c : _npcs) {
		std::cout << "adding npc " << c << "\n";
		// char:
		character = std::make_shared<Character>(src.getSpriteSheet(c), c);
		character->setPosition(spawnPosition);
		// NPC
		std::shared_ptr<NPC> npc = std::make_shared<NPC>();
		npc->setCharacter(character);
		npc->setCheckpoints(checkpoints);
		npcJoin(npc);
		character->setName("NPC " + std::to_string(i++));
		addCharacter(character);
		bool canMoveSpawn = true;
		for (auto t : lvl.getCollidableTiles().tilesToTheSide(character->getHitBox(), false)) {
			canMoveSpawn &= t == Tiles::AIR;
		}
		if (canMoveSpawn) {
			spawnPosition.x -= 20;
		}
	}
	updatePositions();
	for (auto n : npcs) {
		n->setActiveCheckpoint(activeCheckpoint);
	}
	cam.immediateFollow(characters);
	std::cout << characters.size() << " characters now!\n";
	setState(State::Countdown);
	running = true;
}

void Lobby::setState(const State _state)
{
	state = _state;
	if (state == State::Countdown) countdown.reset();
}

Lobby::State Lobby::getState() const{
	return state;
}

int Lobby::getFirstCharacterIdx() const
{
	return 0;
}

void Lobby::loadLevel(const std::string& lvlPath)
{
	lvl.load(lvlPath);
	checkpoints.clear();
	lvl.getCheckpoints(checkpoints);
	std::cout << "I have " << checkpoints.size() << " checkpoints now\n";
	for (NPCPtr npc : npcs) {
		npc->setTileMap(std::make_shared<TileMap>(lvl.getCollidableTiles()));
	}
}

void Lobby::loopGame() {
	float fps, showPeriod = 2;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time currentTime, previousTime = clock.getElapsedTime();

	while (players.size() > 0) {
		update();
		currentTime = clock.getElapsedTime();
		dT = (currentTime - previousTime);
		previousTime = currentTime;
	}
}

void Lobby::checkConnections() {

}

void Lobby::addCharacter(const CharPtr character)
{
	if (characters.size() < 4) {
		characters.emplace_back(character);
		aliveCount++;
	}

}

void Lobby::playerJoin(PlayerPtr newPlayer) {
	if (characters.size() < 4) {
		players.emplace_back(newPlayer);
	}
}

void Lobby::npcJoin(NPCPtr newNPC){
	if (characters.size() < 4) {
		/*Slot s;R
		s.controlIndex = npcs.size();
		s.index = positions.size();
		s.type = 1;*/
		newNPC->setTileMap(std::make_shared<TileMap>(lvl.getCollidableTiles()));
		npcs.emplace_back(newNPC);
		//positions.emplace_back(s);
	}
}

MusicPlayer& Lobby::music() {
	return src.musicPlayer;
}

/*
void Lobby::loop()
{
	// de https://en.sfml-dev.org/forums/index.php?topic=7018.0:
	float fps, showPeriod = 2;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time currentTime, previousTime = clock.getElapsedTime();

	while (window.isOpen()) {
		if (!src.musicPlayer.isPlaying(MusicPlayer::MusicType::REGULAR)) {
			src.musicPlayer.playMusicTrack(MusicPlayer::MusicType::REGULAR);
		}
		if (state == State::FinishedRound) {
			//slow motion
			dT = dT * 0.5f;
		}
		update();
		draw(dT);

		// https://en.sfml-dev.org/forums/index.php?topic=7018.0:
		currentTime = clock.getElapsedTime();
		dT = (currentTime- previousTime);
		fps = 1.0f / dT.asSeconds(); // the asSeconds returns a float
		if ((showPeriod -= dT.asSeconds()) < 0) // Para que no este sacandolo todos los frames
		{
			showPeriod = 2; // cada 2 s
			std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
		}
		previousTime = currentTime;
	}
	running = false;
	clear();
}*/

/*void Lobby::loopMenu()
{
	Resources::getInstance().getAudioPlayer().stopAll();
	Menu menu(window, settings, *this);
	if (!src.musicPlayer.isPlaying(MusicPlayer::MusicType::MENU)) {
		src.musicPlayer.playMusicTrack(MusicPlayer::MusicType::MENU);
	}
	menu.setMainMenu();
	menu.loop(); // , this);
	std::cout << "State after menu: " << (int)state << "\n";
	//state = (Lobby::State)menu.getGameState();
	//characters = menu.getCharacters
}
*/
/*
void Lobby::loopLobby() {
}*/

/*
Lobby::CharPtr Lobby::getCharacterAt(int pos) const {
	return characters[positions[pos].index];
}*/

Lobby::PlayerPtr Lobby::getPlayerAt(int pos) const {
	for (const auto& p : players) {
		if (p->getCharacter() == characters[pos]) return p;
	}
	return nullptr;
}

Lobby::NPCPtr Lobby::getNPCAt(int pos) const {
	for (const auto& npc : npcs) {
		if (npc->getCharacter() == characters[pos]) return npc;
	}
	return nullptr;
}

///////////////////

void Lobby::setOffline() {
	onlineMode = false;
}

void Lobby::setLevelTile(const sf::Vector2i & pos, int tileNumber)
{
	if (!onlineMode) {
		lvl.setTile(pos, tileNumber);
	}
}

void Lobby::drawLevelTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i & pos, int tileNumber) {
	if (!onlineMode) {
		lvl.drawTile(target, states, pos, tileNumber);
	}
	
}

void Lobby::emitParticles(int selectedPSystem, const sf::Vector2f& pos, bool linear, int n) {
	if (!onlineMode) {
		if (linear) {
			particleSystems[selectedPSystem].emitLinear(pos, n);
		}
		else {
			particleSystems[selectedPSystem].emit(pos);
		}
	}
	
}

void Lobby::addCheckpoint(const sf::Vector2f & pos, float radius) {
	if (!onlineMode) {
		checkpoints.emplace_back(pos, radius);
	}
}

void Lobby::removeLastCheckpoint() {
	if (!onlineMode && !checkpoints.empty()) {

	}
}

void Lobby::saveLevelCheckpoints() {
	if (!onlineMode) {
		lvl.setCheckpoints(checkpoints);
	}
}

void Lobby::enableCheats(bool enable) {
	if (!onlineMode) {
		cheatsEnabled = enable;
	}
}

void Lobby::requestAddObject(int type, sf::Vector2f pos, CharPtr newChar) {
	if (!onlineMode) {
		if (type == 0 && newChar != nullptr) { //Character
			addCharacter(newChar);
			lvl.setInitialPosition(pos);
		}
		else if (type == 1) {
			lvl.addBoostBox(pos);
		}
		else if (type == 2) {
			lvl.addItemPickup(pos);
		}
		else if (type == 3) {
			lvl.addBoxObstacle(pos);
		}
	}
}

void Lobby::requestLoadLevel(std::string l) {
	if (!onlineMode) {
		loadLevel(l);
	}
}

void Lobby::requestClearParticles() {
	if (!onlineMode) {
		clearParticles();
	}
}

void Lobby::requestDefaultInit(int N_PLAYERS, const Settings& settings)
{
	if (!onlineMode) {
		defaultInit(N_PLAYERS, settings);
	}
}

void Lobby::requestDefaultInit(const std::vector<glb::characterIndex>& _players, const std::vector<glb::characterIndex>& _npcs) {
	if (!onlineMode) {
		defaultInit(_players, _npcs);
	}
}

void Lobby::requestSetState(const State _state) {
	if (!onlineMode) {
		state = _state;
	}
}

void Lobby::requestSetTestingParticles(bool p) {
	if (!onlineMode) {
		testingParticles = p;
	}
}

/////////////////////

std::vector<particles::PSystem>& Lobby::getParticleSystems() const
{
	return particleSystems;
}

const std::list<Lobby::ItemPtr>& Lobby::getItems() const
{
	return items;
}

const std::shared_ptr<RoundVictory>& Lobby::getRV() const
{
	return rv;
}

const Countdown & Lobby::getCountdown() const
{
	return countdown;
}

Camera Lobby::getCamera() const
{
	return cam;
}

Level Lobby::getLevel() const
{
	return lvl;
}

std::vector<Checkpoint> Lobby::getCheckpoints() const {
	return checkpoints;
}

std::vector<Lobby::NPCPtr> Lobby::getNPCs() const
{
	return npcs;
}

const std::vector<Lobby::CharPtr>& Lobby::getCharacters() const {
	return characters;
}
