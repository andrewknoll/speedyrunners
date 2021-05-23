#include <iostream>
#include <time.h>

#include "Game.h"
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


Game::Game()
	: window(sf::VideoMode(1600, 900), "SpeedyRunners"),
	lvl(window),
	state(State::Countdown),
	selectedTile(Tiles::Collidable::FLOOR),
	cam(sf::FloatRect(0, 0, 1600, 900)),
	src(Resources::getInstance()),
	particleSystems(src.getParticleSystems())
	//dT(0)
{

	setUpWindow();
#ifndef  DISABLE_FULLSCREEN
	setFullScreen();
#else
	settings.setResolution(sf::Vector2i(window.getSize().x, window.getSize().y));
#endif

#ifdef USE_IMGUI
	ImGui::SFML::Init(window);
#endif // USE_IMGUI

	countdown.setWindow(window);
	ui.setWindow(window);

	loadLevel("defaultLevel.csv");

	//;// .push_back(&Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_SMOKE));
}

void Game::clear() {
	gameWon = false;
	Resources::getInstance().getAudioPlayer().stopAll(); // stop all sfx
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

void Game::defaultInit(const std::vector<glb::characterIndex>& _players, const std::vector<glb::characterIndex>& _npcs) {
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
		//  player:
		std::shared_ptr<Player> me = std::make_shared<Player>(getSettings(), i, cheatsEnabled);
		me->setCharacter(character);
		playerJoin(me);
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


void Game::defaultInit(int N_PLAYERS) {

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
	std::shared_ptr<Player> me = std::make_shared<Player>(getSettings(), id, cheatsEnabled);
	me->setCharacter(speedyrunner);
	playerJoin(me);
	speedyrunner->setName("Player 1");
	addCharacter(speedyrunner);

	if (N_PLAYERS > 1) {
		std::shared_ptr<Player> secondPlayer = std::make_shared<Player>(getSettings(), ++id, cheatsEnabled);
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

void Game::setState(const State _state)
{
	state = _state;
	if (state == State::Countdown) countdown.reset();
}

void Game::setUpWindow() {

	window.setFramerateLimit(120); //60 FPS?
	window.setVerticalSyncEnabled(true);
	//auto settings = window.getSettings();
	//settings.antialiasingLevel = 2;
	//window.set
}


// update the positions based on distance to the active checkpoint
void Game::updatePositions()
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

void Game::playerJoin(PlayerPtr newPlayer) {
	if (characters.size() < 4) {
		/*Slot s;
		s.controlIndex = players.size();
		s.index = positions.size();
		s.type = 0;
		*/
		players.emplace_back(newPlayer);
		//positions.emplace_back(s);
	}
}

void Game::npcJoin(NPCPtr newNPC){
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

const Settings& Game::getSettings() const
{
	return settings;
}

int Game::getFirstCharacterIdx() const
{
	return 0;
}

void Game::loadLevel(const std::string& lvlPath)
{
	lvl.load(lvlPath, window);
	checkpoints.clear();
	lvl.getCheckpoints(checkpoints);
	std::cout << "I have " << checkpoints.size() << " checkpoints now\n";
	for (NPCPtr npc : npcs) {
		npc->setTileMap(std::make_shared<TileMap>(lvl.getCollidableTiles()));
	}

}


void Game::setSaveName(std::string fileName) {
	saveLevelName = fileName;
}

void Game::loop()
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
}

void Game::loopMenu()
{
	Resources::getInstance().getAudioPlayer().stopAll();
	Menu menu(window, settings, *this);
	if (!src.musicPlayer.isPlaying(MusicPlayer::MusicType::MENU)) {
		src.musicPlayer.playMusicTrack(MusicPlayer::MusicType::MENU);
	}
	menu.setMainMenu();
	menu.loop(); // , this);
	std::cout << "State after menu: " << (int)state << "\n";
	//state = (Game::State)menu.getGameState();
	//characters = menu.getCharacters
}

void Game::addCharacter(const CharPtr character)
{
	if (characters.size() < 4) {
		characters.emplace_back(character);
		ui.setCharacters(characters);
		aliveCount++;
	}

}
void Game::createNewLevel(int nLevels)
{
	loadLevel("default_level.csv");
	saveLevelName = "USER_LEVEL_" + std::to_string(nLevels) + ".csv";
}
/*
Game::CharPtr Game::getCharacterAt(int pos) const {
	return characters[positions[pos].index];
}*/

Game::PlayerPtr Game::getPlayerAt(int pos) const {
	for (const auto& p : players) {
		if (p->getCharacter() == characters[pos]) return p;
	}
	return nullptr;
}

Game::NPCPtr Game::getNPCAt(int pos) const {
	for (const auto& npc : npcs) {
		if (npc->getCharacter() == characters[pos]) return npc;
	}
	return nullptr;
}

MusicPlayer& Game::music() {
	return src.musicPlayer;
}

void Game::updateNPCs(bool follow) {
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
				//threadPool[i]->detach();
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
				//threadPool[i]->detach();
			}
			if (follow && state == State::Playing) {
				npcs[i]->update(dT);
			}
			/**
			if (follow && threadPool[2 * i + 2].threadPtr == nullptr) {
				threadPool[2 * i + 2].finished = false;
				threadPool[2 * i + 2].threadPtr = std::make_unique<std::thread>([&, i]() {
					while (running) {
						if (npcs[i]->getCharacter()->isDead()) {
							{ std::unique_lock<std::mutex> lck(restartMtx);
							restartCv.wait(lck); }
						}
						if (running) {
							if (npcs[i]->getPathFound(0) == 1) {
								npcs[i]->followPath();
							}
							//else {
							//	npcs[i]->plan();
							//}
						}
					}
					threadPool[2 * i + 2].finished = true;
					finishCV.notify_one();
				});
			}*/
		}
	}
}

void Game::setFullScreen() {
	sf::VideoMode vMode = sf::VideoMode::getFullscreenModes().front();
	if (!vMode.isValid()) {
		std::cerr << "NOPE\n";
	}
	window.create(vMode, "SpeedyRunners", sf::Style::Fullscreen);
	setUpWindow();
	cam = window.getDefaultView();
	lvl.loadBackground(lvl.getBackgroundPath(), window);
	settings.setResolution(sf::Vector2i(window.getSize().x, window.getSize().y));
}


void Game::updateItems() {
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
		if(handle != 1) {
			++itr;
		}
	}
}

void Game::clearParticles() {
	for (auto& ps : particleSystems) ps.clear();
}

void Game::update()
{
	sf::Event event;
	int target;
	while (window.pollEvent(event))
	{
#ifdef USE_IMGUI
		ImGui::SFML::ProcessEvent(event);
#endif // USE_IMGUI

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			loopMenu();
		}
		else if (event.type == sf::Event::Closed) {
			running = false;
			window.close();
		}


		
		if (event.type == sf::Event::Resized)
		{
			// sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			sf::FloatRect visibleArea(0, 0, float(event.size.width), float(event.size.width) / aspectRatio);

			window.setView(sf::View(visibleArea));
		}
		if (cheatsEnabled) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
				state = State::Playing;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
				state = State::Editing;
				addingCheckpoint = false;
				testingParticles = false;
				clearParticles();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F4) {
				loopMenu();
			}
		}


		if (state == State::Editing) { // Editing state
			processEditingInputs(event);
		} // End of editing state
		else if (state == State::Playing) { // Playing
			//characters.front().processInput(event); // Podemos cambiarlo por Player en el futuro
			for (int i = 0; i < characters.size(); i++) {
				target = i; //Set target initial value to oneself

				if (i > 0) target--;
				else if (i < characters.size() - 1) target++;
				//if player is dumb and uses rockets when nobody else is playing, it will hit them
				auto p = getPlayerAt(i);
				auto n = getNPCAt(i);
				if (p != nullptr && p->captureEvents(event)
					|| n != nullptr && n->getAndResetUseItem()) {
					auto item = characters[i]->getCurrentItem();
					if (item == glb::item::ROCKET) {
						auto rocket = std::make_shared<Rocket>(characters[i]->getPosition(), characters[target], characters[i]->isFacingRight());
						
						items.push_back(rocket);
						if (!cheatsEnabled && p != nullptr) characters[i]->resetItem();
					} 
					else if ((int)item >= glb::NUMBER_OF_ITEMS-glb::NUMBER_OF_UNOBTAINABLE_ITEMS 
							&& (int)item < glb::NUMBER_OF_ITEMS) { // crates
						lvl.dropCrate(characters[i]->getBackPosition());
						characters[i]->setItem(glb::item(((int)item + 1) % (glb::NUMBER_OF_ITEMS)));
					}
					else if (item == glb::TNT) {
						auto& tnt = items.emplace_back(std::make_shared<TNT>(characters[i]->getBackPosition(), lvl.getCollidableTiles().getTileSizeWorld().x));
						characters[i]->setItemPtr(tnt);
						characters[i]->setItem(glb::item::TNT_DETONATOR);
					}
					else if (item == glb::TNT_DETONATOR) {
						characters[i]->getItemPtr()->changeState();
						characters[i]->resetItem();
					}
					else if (item == glb::ICERAY) {
						auto iceray = std::make_shared<IceRay>(cam, characters[i]);
						items.push_back(iceray);
						if (!cheatsEnabled && p != nullptr) characters[i]->resetItem();
					}
					else if (item == glb::GOLDEN_HOOK) {
						auto ghook = std::make_shared<GoldenHook>(characters[i], characters[target]);
						items.push_back(ghook);
						if (!cheatsEnabled && p != nullptr) characters[i]->resetItem();
					}
					else {
						std::cout << "unimplemented item " << item << "\n";
						characters[i]->setItem(glb::NONE);
					}
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10)) { // Change to fullscreen (change back to window not implemented)
			setFullScreen();
		}

	}
#ifdef USE_IMGUI
	ImGui::SFML::Update(window,dT);
#endif
	processMouseEditing();
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
					ui.updatePoints();
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
					rv = std::make_unique<RoundVictory>(window, characters[i]->getID(), characters[i]->getVariant(), characters[i]->getScore());
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
					loopMenu();
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

void Game::handleItem(Game::ItemPtr item) {
	for (auto& c : characters) {
		item->doThingTo(c);
	}
}

void Game::enableCheats(bool enable) {
#ifdef DEV_MODE
	cheatsEnabled = true;
#else
	cheatsEnabled = enable;
#endif
}

void Game::processMouseEditing() {
	if (state == State::Editing && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (!addingCheckpoint && !testingParticles)
			lvl.setTile(utils::clampMouseCoord(window), selectedTile);
		else if (!addingCheckpoint) {
			particleSystems[selectedPSystem].emit(utils::mousePosition2f(window));
		}
	}

}

// Controls for editing state:
void Game::processEditingInputs(const sf::Event& event) {
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		cam.moveByMouse(sf::Mouse::getPosition());
		//cam.move(sf::Vector2f(1, 0));
	}
	else {
		cam.resetDrag();
	}
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
			int add = -1;
			if (event.mouseWheelScroll.delta > 0) {
				add = 1;
			}
			selectedTile = (Tiles::Collidable)utils::positiveMod((selectedTile + add), Tiles::NB_COLLIDABLE);
			//std::cout << "New tile selected: " << selectedTile << std::endl;
		}
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad7) {
		testingParticles = !testingParticles;
		addingCheckpoint = false;
		clearParticles();
	}
	else if (testingParticles && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad8) {
		selectedPSystem = (selectedPSystem + 1) % particleSystems.size();
		std::cout << "selected PSystem: " << selectedPSystem << "\n";
	}
	else if (testingParticles && event.type == sf::Event::MouseButtonPressed && event.key.code ==sf::Mouse::Middle)
		//particleSystems[selectedPSystem].burstOut(utils::mousePosition2f(window), 200, 100);
		particleSystems[selectedPSystem].emitLinear(utils::mousePosition2f(window), 1000);
	else if (addingCheckpoint && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
		std::cout << "adding checkpoint to " << utils::mousePosition2f(window).x << " " << utils::mousePosition2f(window).y << "\n";
		checkpoints.emplace_back(utils::mousePosition2f(window), currentRadius);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad4) { // box debug
		lvl.testBoxCollision(utils::mousePosition2f(window));
	}
	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			lvl.setCheckpoints(checkpoints);
			//lvl.saveDuplicateVertical("first.csv");

			//lvl.setDefaultLevel();
			//lvl.save("default_level.csv");

			lvl.save(saveLevelName);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			loadLevel("defaultLevel.csv");
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) {

			// Join as player
			//playerJoin();
		}
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
		addingCheckpoint = !addingCheckpoint;
		testingParticles = false;
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && !checkpoints.empty()) {
		std::cout << "removing last checkpoint\n";
		checkpoints.pop_back();
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		//std::cout << "Resizing circle...\n";
		currentRadius += 10;
		checkpointCircle.setRadius(currentRadius);
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && currentRadius > 10) {
		//std::cout << "Resizing circle...\n";
		currentRadius -= 10;
		checkpointCircle.setRadius(currentRadius);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) { // For debug, add Falcon to mouse position
		// Add player
		Spritesheet sprsht = src.getSpriteSheet(3); //Falcon
		std::shared_ptr<Character> falcon = std::make_shared<Character>(sprsht, glb::FALCON);
		falcon->setPosition(utils::mousePosition2f(window));
		falcon->setName(std::string("falcon ") + std::to_string(characters.size()));
		//falcon.setScale(0.5, 0.5);
		addCharacter(falcon);
		lvl.setInitialPosition(falcon->getPosition());
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
		lvl.addBoostBox(utils::mousePosition2f(window));
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) { // ITem pickup
		lvl.addItemPickup(utils::mousePosition2f(window));
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) { // box obstacle
		lvl.addBoxObstacle(utils::mousePosition2f(window));
	}

	// Debug player positions (P to show):
	printCharacterPositions(event);

}

void Game::printCharacterPositions(const sf::Event& e) const {

	if (e.type == sf::Event::KeyPressed && e.key.code == (sf::Keyboard::P)) {
		std::cout << "Character positions:\n";
		for (auto c : characters)  std::cout << c->getName() << " | ";
		std::cout << "\nCharacter distances:\n";
		for (auto c : characters) std::cout << c->getDToCheckpoint() << " | ";
		std::cout << "\n";
	}
}


void Game::animateCharacters() {
	for (auto c : characters) {
		if (!c->isDead()) {
			c->tickAnimation(dT);
			window.draw(*c);
		}
	}
}

void Game::draw(sf::Time dT)
{
	window.clear();
	window.setView(cam);
	window.draw(lvl);

	/*for (auto npc : npcs) {
		window.draw(npc->debugCurrentPos());
	}*/

	if(state == State::Editing) {
		// sf::Mouse::getPosition() - window.getPosition()
		// Visualizar checkpoints:
		for (auto cp : checkpoints) {
			window.draw(cp);
		}
		// characters (sin hacer tick a las animaciones):
		/*for (auto c : characters) {
			if (!c->isDead()) {
				window.draw(*c);
			}
		}*/
		for (auto npc : npcs) {
			for (auto l : npc->debugExpanded()) {
				window.draw(l);
			}
			for (auto l : npc->debugLines()) {
				window.draw(l);
			}
		}
		// Selected tile
		if (!addingCheckpoint && !testingParticles)
			lvl.drawTile(window, sf::RenderStates(), utils::clampMouseCoord(window), selectedTile);
		else if (addingCheckpoint) {
			checkpointCircle.setPosition(utils::mousePosition2f(window));
			window.draw(checkpointCircle);
		}
		/*else for (const auto& ps : particleSystems) {
			window.draw(ps); // testing particles
		}*/
	}

	// Characters
	
	animateCharacters();
	for (const auto& ps : particleSystems) window.draw(ps);
	for (const auto i : items) {
		window.draw(*i);
	}

	//Sudden death screen
	if (state != State::Editing) {
		window.setView(window.getDefaultView());
		window.draw(cam);
	}

	if (state == State::FinishedRound) {
		if (rv != nullptr) {
			rv->draw(window);
		}
	}
	else if (state == State::Countdown) {
		suddenDeath = false;
		cam.setSuddenDeath(false);
		countdown.draw(window);
		//animateCharacters();
	}

	// UI
	ui.update();
	window.draw(ui);

#ifdef USE_IMGUI
	ImGui::Begin("Hello, world!");
	ImGui::Button("Look at this pretty button");
	ImGui::End();

	ImGui::SFML::Render(window);
#endif // USE_IMGUI

	window.display();
}
