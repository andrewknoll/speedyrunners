#include <iostream>
#include <time.h>

#include "Game.h"
#include "utils.hpp"
#include "Player.h"
#include "Menu.h"


//#define VERBOSE_DEBUG // Cambiar para quitar couts

Game::Game()
	: window(sf::VideoMode(1600, 900), "SpeedyRunners"),
	lvl(window),
	state(State::Playing),
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
	for (auto c : characters) {
		float d = utils::distance(c->getPosition(), cpPos);
		c->setDToCheckpoint(d);
		// get distance d to active checkpoint
		// if d < r: next checkpoint
	}
	// order characters and players based on distance
	std::sort(positions.begin(), positions.end(),
		[chars = this->characters](Slot i, Slot j) {
			//TODO- MUERTE
			return chars[i.index]->getDToCheckpoint() < chars[j.index]->getDToCheckpoint();
		}
	);
	// Check if one has reached the checkpoint
	if (getCharacterAt(0)->getDToCheckpoint() <= cp.getRadius()) {
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
	if (positions.size() < 4) {
		Slot s;
		s.controlIndex = players.size();
		s.index = positions.size();
		s.type = 0;

		players.emplace_back(newPlayer);
		positions.emplace_back(s);
	}
}

void Game::npcJoin(NPCPtr newNPC){
	if (positions.size() < 4) {
		Slot s;
		s.controlIndex = npcs.size();
		s.index = positions.size();
		s.type = 1;
		newNPC->setTileMap(std::make_shared<TileMap>(lvl.getCollidableTiles()));
		npcs.emplace_back(newNPC);
		positions.emplace_back(s);
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
		//TO-DO: Sudden Death
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
}

void Game::loopMenu()
{
	Menu menu(window);
	if (!src.musicPlayer.isPlaying(MusicPlayer::MusicType::MENU)) {
		src.musicPlayer.playMusicTrack(MusicPlayer::MusicType::MENU);
	}
	menu.setMainMenu(settings);
	menu.loop(settings); // , this);
}

void Game::addCharacter(const CharPtr character)
{
	if (characters.size() < 4) {
		characters.emplace_back(character);
		ui.setCharacters(characters);
	}
	
}

Game::CharPtr Game::getCharacterAt(int pos) {
	return characters[positions[pos].index];
}

Game::PlayerPtr Game::getPlayerAt(int pos) {
	if (positions[pos].type == 0) {
		return players[positions[pos].controlIndex];
	}
	return nullptr;
}

Game::NPCPtr Game::getNPCAt(int pos) {
	if (positions[pos].type == 1) {
		return npcs[positions[pos].controlIndex];
	}
	return nullptr;
}

MusicPlayer& Game::music() {
	return src.musicPlayer;
}

void Game::update()
{
	sf::Event event;
	int target;
	if (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();
		if (event.type == sf::Event::Resized)
		{
			// sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			sf::FloatRect visibleArea(0, 0, float(event.size.width), float(event.size.width) / aspectRatio);

			window.setView(sf::View(visibleArea));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
			state = State::Playing;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
			state = State::Editing;
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F4) {
			loopMenu();
		}
		if (state == State::Editing) { // Editing state
			processEditingInputs(event);
		} // End of editing state
		else if (state == State::Playing) { // Playing
			//characters.front().processInput(event); // Podemos cambiarlo por Player en el futuro
			for (int i = 0; i < characters.size(); i++) {
				target = i; //Set target initial value to oneself
				
				if (positions[i].type == 0 && getPlayerAt(i)->captureEvents(event)) {
					if (i > 0) target--;
					else if (i < characters.size() - 1) target++;
					//if player is dumb and uses rockets when nobody else is playing, it will hit them
					items.push_back(getCharacterAt(i)->useItem(getCharacterAt(target)));
				};
				if (positions[i].type == 1) {
					Checkpoint cp = checkpoints[activeCheckpoint];
					getNPCAt(i)->play(cp.getPos(), cp.getRadius());
				}
				//TODO: PLAY NPC
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
			c->update(dT, lvl.getCollidableTiles());
		}
		for (auto it : items) {
			it->update(dT);
		}
		updatePositions();
		cam.follow(characters); 
	}
	else if (state == State::Countdown) {
		countdown.update(dT);
		if (countdown.ended()) {
			state = State::Playing;
		}
	}
	//cam.pollEvents();
	// TODO
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
			lvl.save("first.csv");
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
		Spritesheet sprsht;
		sprsht.parse_spritesheet("../assets/Content/Characters/Falcon/animation_variant01.png", "../assets/indexes/Characters/Falcon/animation_atlas_variant00.json");
		std::shared_ptr<Character> falcon = std::make_shared<Character>(sprsht); //"../assets/Content/Characters/Falcon/");
		falcon->setPosition(utils::mousePosition2f(window));
		falcon->setName(std::string("falcon ") + std::to_string(characters.size()));
		//falcon.setScale(0.5, 0.5);
		addCharacter(falcon);

	} 
	
	// Debug player positions (P to show):
	printCharacterPositions(event);

}

void Game::printCharacterPositions(const sf::Event& e) const {
	
	if (e.type == sf::Event::KeyPressed && e.key.code == (sf::Keyboard::P)) {
		std::cout << "Character positions:\n";
		for (auto c : characters) {
			std::cout << c->getName() << " | ";
		}
		std::cout << "\nCharacter distances:\n";
		for (auto c : characters) {
			std::cout << c->getDToCheckpoint() << " | ";
		}
		std::cout << "\n";
	}
}



void Game::draw(sf::Time dT)
{
	window.clear();
	window.setView(cam);
	window.draw(lvl);
	switch (state) {
	case State::Editing:
	{// sf::Mouse::getPosition() - window.getPosition()
		// Visualizar checkpoints:
		for (auto cp : checkpoints) {
			window.draw(cp);
		}
		// characters (sin hacer tick a las animaciones):
		for (auto c : characters) {
			window.draw(*c);
		}
		for (auto npc : npcs) {
			for (auto l : npc->debugLines()) {
				window.draw(l);
			}
			
		}
		// Selected tile 
		lvl.drawTile(window, sf::RenderStates(), utils::clampMouseCoord(window), selectedTile);
		break;
	}
	case State::Countdown:
	{
		countdown.draw(window);
		//window.draw(countdown);
		
	} // no break, we also animate the characters
	case State::Playing:
	{ // Animaciones de personajes:
		for (auto c : characters) {
			c->tickAnimation(dT);
			window.draw(*c);
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

