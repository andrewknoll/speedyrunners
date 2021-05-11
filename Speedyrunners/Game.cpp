#include <iostream>
#include <time.h>

#include "Game.h"
#include "utils.hpp"
#include "Player.h"
#include "Menu.h"
#include "RoundVictory.h"


//#define VERBOSE_DEBUG // Cambiar para quitar couts

Game::Game()
	: window(sf::VideoMode(1600, 900), "SpeedyRunners"),
	lvl(window),
	state(State::Countdown),
	selectedTile(Tiles::Collidable::FLOOR),
	cam(sf::FloatRect(0, 0, 1600, 900)),
	countdown(window),
	src(Resources::getInstance())
	//dT(0)
{
	setUpWindow();

	ui.setWindow(window);

	loadLevel("first.csv");
	//TO-DO: Utilizar "sets" de musica predeterminados
	//o asignarlas al nivel


	settings.setResolution(sf::Vector2i(1600, 900));

	// DEBUG de menus:
	//loopMenu();
}

void Game::clear() {
	characters.clear();
	npcs.clear();
	players.clear();
	running = false;
	threadPool.clear();
	countdown.end();


	threadPool.resize(8);
}

void Game::defaultInit(int N_PLAYERS) {

	if (N_PLAYERS != 2) N_PLAYERS = 1;// only 1 or 2 players
	clear();

	std::shared_ptr<Character> speedyrunner = std::make_shared<Character>(src.getSpriteSheet(0), glb::SPEEDRUNNER);
	speedyrunner->setPosition(lvl.getInitialPosition());

	std::shared_ptr<Character> cosmonaut = std::make_shared<Character>(src.getSpriteSheet(1), glb::COSMONAUT);
	cosmonaut->setPosition(lvl.getInitialPosition());

	std::shared_ptr<Character> otro = std::make_shared<Character>(src.getSpriteSheet(2), glb::UNIC);
	otro->setPosition(lvl.getInitialPosition());

	int id = 0;
	if (N_PLAYERS == 2) id = 1;
	std::shared_ptr<Player> me = std::make_shared<Player>(getSettings(), id);
	me->setCharacter(speedyrunner);
	playerJoin(me);
	speedyrunner->setName("Player 1");
	addCharacter(speedyrunner);

	if (N_PLAYERS > 1) {
		std::shared_ptr<Player> secondPlayer = std::make_shared<Player>(getSettings(), ++id);
		secondPlayer->setCharacter(otro);
		playerJoin(secondPlayer);
		otro->setName("Player 2");
		addCharacter(otro);
	}
	std::shared_ptr<NPC> other = std::make_shared<NPC>();
	other->setCharacter(cosmonaut);
	npcJoin(other);
	cosmonaut->setName("NPC");
	addCharacter(cosmonaut);
	setState(State::Countdown);
	running = true;
}

void Game::setState(const State _state)
{
	state = _state;
	if (state == State::Countdown) countdown.reset();
}

void Game::setUpWindow() {

	window.setFramerateLimit(60); //60 FPS?
	window.setVerticalSyncEnabled(true);
	//auto settings = window.getSettings();
	//settings.antialiasingLevel = 2;
	//window.set
}


// update the positions based on distance to the active checkpoint
void Game::updatePositions()
{
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
		std::cout << "Checkpoint " << activeCheckpoint <<" reached\n";
#endif
		activeCheckpoint = (activeCheckpoint + 1) % checkpoints.size();
#ifdef VERBOSE_DEBUG
		std::cout << "(new = " << activeCheckpoint << ")\n";
#endif
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
		/*Slot s;
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
	lvl.getCheckpoints(checkpoints);
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
}

void Game::loopMenu()
{
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

void Game::updateNPCs() {
	for (int i = 0; i < npcs.size(); i++) {
		if (npcs[i] != nullptr) {
			Checkpoint cp = checkpoints[activeCheckpoint]; // checkpoints[1];
			npcs[i]->addGoal(cp.getPos(), cp.getRadius());
			cp = checkpoints[(activeCheckpoint + 1) % checkpoints.size()];
			npcs[i]->addGoal(cp.getPos(), cp.getRadius());
			auto& followThread = threadPool[2 * i + 1];
			if (threadPool[2 * i] == nullptr) {
				threadPool[2 * i] = std::make_unique<std::thread>([&, i]() {
					while (running && !npcs[i]->getCharacter()->isDead()) {
						npcs[i]->plan();
					}
				});
				//threadPool[i]->detach();
			}
			if (threadPool[2 * i + 1] == nullptr) {
				threadPool[2 * i + 1] = std::make_unique<std::thread>([&, i]() {
					while (running && !npcs[i]->getCharacter()->isDead()) {
						if (npcs[i]->getPathFound(0) == 1) {
							npcs[i]->followPath();
						}
						else {
							npcs[i]->plan();
						}
					}
				});
			}
		}
	}
}

void Game::update()
{
	sf::Event event;
	int target;
	if (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			running = false;
			threadPool.clear();
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
				auto p = getPlayerAt(i);
				if (p != nullptr && p->captureEvents(event)) {
					if (i > 0) target--;
					else if (i < characters.size() - 1) target++;
					//if player is dumb and uses rockets when nobody else is playing, it will hit them
					items.push_back(characters[i]->useItem(characters[target]));
				};
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10)) { // Change to fullscreen (change back to window not implemented)
			sf::VideoMode vMode = sf::VideoMode::getFullscreenModes().front();
			if (!vMode.isValid()) {
				std::cerr << "NOPE\n";
			}
			window.create(vMode, "SpeedyRunners", sf::Style::Fullscreen);
			setUpWindow();
			cam = window.getDefaultView();
			lvl.loadBackground(lvl.getBackgroundPath(), window);
		}

	}
	if (state == State::Playing && dT.asSeconds() < 0.1) {
		for (auto c : characters) {
			if (c->isDead()) continue;
			c->update(dT, lvl.getCollidableTiles());
		}
		for (auto it : items) {
			it->update(dT);
		}
		updatePositions();
		cam.follow(characters);
		cam.update(dT);
		for (int i = 0; i < characters.size(); i++) {
			if (characters[i]->isDead()) continue;
			if (!cam.isInAllowedBounds(characters[i])) {
				characters[i]->die();
				src.getAudioPlayer().play(AudioPlayer::Effect::CHARACTER_OUTOFSCREEN);
				src.getAudioPlayer().play(AudioPlayer::Effect::DEATH);
				aliveCount--;
				suddenDeath = true;
				cam.setSuddenDeath(true);
			}
		}
		updateNPCs();
		if (aliveCount == 1) {
			state = State::FinishedRound;
			for (auto c : characters) {
				if (!c->isDead()) {
					c->increaseScore(1);
					break;
				}
				//TODO : Ganar con tres puntos
			}
		}

	}
	else if (state == State::Countdown) {
		updateNPCs();
		countdown.update(dT);
		if (countdown.ended()) {
			state = State::Playing;
		}
	}
	else if (state == State::FinishedRound) {
		cam.follow(characters);
		cam.update(dT);
		if (rv == nullptr) {
			for (int i = 0; i < characters.size(); i++) {
				if (!characters[i]->isDead()) {
					rv = std::make_unique<RoundVictory>(window, characters[i]->getID(), characters[i]->getVariant(), characters[i]->getScore());
					respawnPosition = characters[i]->getLastSafePosition();
				}
				//TODO : Ganar con tres puntos
			}
		}
		else {
			rv->update(dT);
			if (rv->ended()) {
				state = State::Countdown;
				countdown.reset();
				rv = nullptr;
				suddenDeath = false;
				cam.setSuddenDeath(false);
				aliveCount = characters.size();
				for (int i = 0; i < characters.size(); i++) {
					characters[i]->respawn(respawnPosition);
				}
			}
		}
	}
	//cam.pollEvents();
	// TODO
}

void Game::enableCheats(bool enable) {
	cheatsEnabled = enable;
}

// Controls for editing state:
void Game::processEditingInputs(const sf::Event& event) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		lvl.setTile(utils::clampMouseCoord(window), selectedTile);
	}
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			lvl.setCheckpoints(checkpoints);
			//lvl.saveDuplicateVertical("first.csv");
			
			//lvl.setDefaultLevel();
			//lvl.save("default_level.csv");

			lvl.save(saveLevelName);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			loadLevel("first.csv");
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) {

			// Join as player
			//playerJoin();
		}
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
		std::cout << "adding circle to " << utils::mousePosition2f(window).x << " " << utils::mousePosition2f(window).y<< "\n";
		checkpoints.emplace_back(utils::mousePosition2f(window), currentRadius);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && !checkpoints.empty()) {
		std::cout << "removing last checkpoint\n";
		checkpoints.pop_back();
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		std::cout << "Resizing circle...\n";
		currentRadius += 10;
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && currentRadius > 10) {
		std::cout << "Resizing circle...\n";
		currentRadius -= 10;
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



void Game::draw(sf::Time dT)
{
	window.clear();
	window.setView(cam);
	window.draw(lvl);

	if (suddenDeath) {
		window.draw(cam.getSuddenDeathRectangle());
	}

	switch (state) {
	case State::Editing:
	{// sf::Mouse::getPosition() - window.getPosition()
		// Visualizar checkpoints:
		for (auto cp : checkpoints) {
			window.draw(cp);
		}
		// characters (sin hacer tick a las animaciones):
		for (auto c : characters) {
			if (!c->isDead()) {
				window.draw(*c);
			}
		}
		for (auto npc : npcs) {
			for (auto l : npc->debugExpanded()) {
				window.draw(l);
			}
			for (auto l : npc->debugLines()) {
				window.draw(l);
			}

		}
		// Selected tile
		lvl.drawTile(window, sf::RenderStates(), utils::clampMouseCoord(window), selectedTile);
		break;
	}
	case State::FinishedRound:
	{
		if (rv != nullptr) {
			rv->draw(window);
		}
	}
	case State::Countdown:
	{
		if (state == State::Countdown) {
			countdown.draw(window);
		}

	} // no break, we also animate the characters
	case State::Playing:
	{ // Animaciones de personajes:
		for (auto c : characters) {
			if (!c->isDead()) {
				c->tickAnimation(dT);
				window.draw(*c);
			}
		}
		for (auto i : items) {
			window.draw(*i);
		}
		// UI:
		window.draw(ui);
		break;
	}
	default:
	{
		//std::cout << "unknown game state\n" << (int)state;
	}
	}


	//lvl.draw(window, cam);
	//window.draw();
	window.display();
}
