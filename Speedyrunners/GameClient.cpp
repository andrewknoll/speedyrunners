#include "GameClient.h"
#include "Menu.h"
#include "Level.h"
#include <iostream>


void GameClient::setUpWindow() {

	window.setFramerateLimit(60); //60 FPS?
	window.setVerticalSyncEnabled(true);
	//auto settings = window.getSettings();
	//settings.antialiasingLevel = 2;
	//window.set
}


GameClient::GameClient() :
	window(sf::VideoMode(1600, 900), "SpeedyRunners"),
	src(Resources::getInstance())
{
}


void GameClient::createLobby() {
	lobby = std::make_shared<LobbyInterface>();
	onlineLobby = false;
}


void GameClient::setFullScreen() {
	sf::VideoMode vMode = sf::VideoMode::getFullscreenModes().front();
	if (!vMode.isValid()) {
		std::cerr << "NOPE\n";
	}
	window.create(vMode, "SpeedyRunners", sf::Style::Fullscreen);
	setUpWindow();
	//todo
	if (!onlineLobby) {
		
	}
	bg.load(window);
	settings.setResolution(sf::Vector2i(window.getSize().x, window.getSize().y));
}


void GameClient::offlineUpdate(sf::Time dT) {
	sf::Event event;
	int target;
	while (window.pollEvent(event))
	{
#ifdef USE_IMGUI
		ImGui::SFML::ProcessEvent(event);
#endif // USE_IMGUI

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			onlineLobby = false;
			socketToServer->disconnect();
			loopMenu();
		}
		else if (event.type == sf::Event::Closed) {
			socketToServer->disconnect();
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
				lobby->requestClearParticles();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F4) {
				loopMenu();
			}
		}


		if (state == State::Editing) { // Editing state
			processEditingInputs(event);
		if (state == State::Editing && testingParticles && dT.asSeconds() < 0.1)
			for (auto& ps : lobby->getParticleSystems()) ps.update(dT); // update particles
		} // End of editing state
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10)) { // Change to fullscreen (change back to window not implemented)
			setFullScreen();
		}

	}
#ifdef USE_IMGUI
	ImGui::SFML::Update(window, dT);
#endif
	processMouseEditing();
}


void GameClient::loopMenu()
{
	Resources::getInstance().getAudioPlayer().stopAll();
	Menu menu(window, settings, lobby, *this);
	if (!src.musicPlayer.isPlaying(MusicPlayer::MusicType::MENU)) {
		src.musicPlayer.playMusicTrack(MusicPlayer::MusicType::MENU);
	}
	menu.setMainMenu();
	menu.loop(); // , this);
	std::cout << "State after menu: " << (int)state << "\n";
	//state = (LobbyInterface::State)menu.getGameState();
	//characters = menu.getCharacters
}


void GameClient::processMouseEditing() {
	if (state == State::Editing && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (!addingCheckpoint && !testingParticles)
			lobby->setLevelTile(utils::clampMouseCoord(window), selectedTile);
		else if (!addingCheckpoint) {
			lobby->emitParticles(selectedPSystem, utils::mousePosition2f(window));
		}
	}

}

// Controls for editing state:

void GameClient::processEditingInputs(const sf::Event& event) {

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
		lobby->requestClearParticles();
	}
	else if (testingParticles && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad8) {
		selectedPSystem = (selectedPSystem + 1) % lobby->getParticleSystems(socketToServer).size();
		std::cout << "selected PSystem: " << selectedPSystem << "\n";
	}
	else if (testingParticles && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Middle)
		//particleSystems[selectedPSystem].burstOut(utils::mousePosition2f(window), 200, 100);
		lobby->emitParticles(selectedPSystem, utils::mousePosition2f(window), true, 1000);
	else if (addingCheckpoint && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
		std::cout << "adding checkpoint to " << utils::mousePosition2f(window).x << " " << utils::mousePosition2f(window).y << "\n";
		lobby->addCheckpoint(utils::mousePosition2f(window), currentRadius);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad4) { // box debug
		lobby->getLevel(socketToServer).testBoxCollision(utils::mousePosition2f(window));
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { // sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			lobby->saveLevelCheckpoints();
			//lvl.saveDuplicateVertical("first.csv");

			//lvl.setDefaultLevel();
			//lvl.save("default_level.csv");

			lobby->getLevel(socketToServer).save(saveLevelName);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			lobby->requestLoadLevel("defaultLevel.csv");
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
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace) {
		std::cout << "removing last checkpoint\n";
		lobby->removeLastCheckpoint();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		//std::cout << "Resizing circle...\n";
		currentRadius += 10;
		checkpointCircle.setRadius(currentRadius);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && currentRadius > 10) {
		//std::cout << "Resizing circle...\n";
		currentRadius -= 10;
		checkpointCircle.setRadius(currentRadius);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) { // For debug, add Falcon to mouse position
		// Add player
		Spritesheet sprsht = src.getSpriteSheet(3); //Falcon
		std::shared_ptr<Character> falcon = std::make_shared<Character>(sprsht, glb::FALCON);
		falcon->setPosition(utils::mousePosition2f(window));
		falcon->setName(std::string("falcon ") + std::to_string(lobby->getCharacters(socketToServer).size()));
		//falcon.setScale(0.5, 0.5);
		lobby->requestAddObject(0, falcon->getPosition(), falcon);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
		lobby->requestAddObject(1, utils::mousePosition2f(window));
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) { // ITem pickup
		lobby->requestAddObject(2, utils::mousePosition2f(window));
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) { // box obstacle
		lobby->requestAddObject(3, utils::mousePosition2f(window));
	}

	// Debug player positions (P to show):
	printCharacterPositions(event);
}


void GameClient::setState(const State _state)
{
	state = _state;
}


GameClient::State GameClient::getState() const {
	return state;
}


void GameClient::printCharacterPositions(const sf::Event& e) const {

	if (e.type == sf::Event::KeyPressed && e.key.code == (sf::Keyboard::P)) {
		std::cout << "Character positions:\n";
		for (auto c : lobby->getCharacters(socketToServer))  std::cout << c->getName() << " | ";
		std::cout << "\nCharacter distances:\n";
		for (auto c : lobby->getCharacters(socketToServer)) std::cout << c->getDToCheckpoint() << " | ";
		std::cout << "\n";
	}
}


void GameClient::draw(sf::Time dT)
{
	window.clear();
	window.setView(cam);
	window.draw(lobby->getLevel(socketToServer));

	/*for (auto npc : npcs) {
		window.draw(npc->debugCurrentPos());
	}*/

	if(state == State::Editing) {
		// sf::Mouse::getPosition() - window.getPosition()
		// Visualizar checkpoints:
		for (auto cp : lobby->getCheckpoints(socketToServer)) {
			window.draw(cp);
		}
		// characters (sin hacer tick a las animaciones):
		/*for (auto c : characters) {
			if (!c->isDead()) {
				window.draw(*c);
			}
		}*/
		for (auto npc : lobby->getNPCs(socketToServer)) {
			for (auto l : npc->debugExpanded()) {
				window.draw(l);
			}
			for (auto l : npc->debugLines()) {
				window.draw(l);
			}
		}
		// Selected tile
		if (!addingCheckpoint && !testingParticles)
			lobby->drawLevelTile(window, sf::RenderStates(), utils::clampMouseCoord(window), selectedTile);
		else if (addingCheckpoint) {
			checkpointCircle.setPosition(utils::mousePosition2f(window));
			window.draw(checkpointCircle);
		}
		/*else for (const auto& ps : particleSystems) {
			window.draw(ps); // testing particles
		}*/
	}

	// Characters
	
	animateCharacters(dT);
	for (const auto& ps : lobby->getParticleSystems(socketToServer)) window.draw(ps);
	for (const auto i : lobby->getItems(socketToServer)) {
		window.draw(*i);
	}

	//Sudden death screen
	if (state != State::Editing) {
		window.setView(window.getDefaultView());
		window.draw(cam);
	}

	if (state == State::FinishedRound) {
		auto rvPointer = lobby->getRV(socketToServer);
		if (rvPointer != nullptr) {
			rvPointer->setWindow(window);
			rvPointer->draw(window);
		}
	}
	else if (state == State::Countdown) {
		lobby->getCountdown(socketToServer).draw(window);
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


void GameClient::enableCheats(bool enable) {
#ifdef DEV_MODE
		cheatsEnabled = true;
#else
		cheatsEnabled = enable;
#endif
		lobby->enableCheats(cheatsEnabled);
}


void GameClient::animateCharacters(sf::Time dT) {
	for (auto c : lobby->getCharacters(socketToServer)) {
		if (!c->isDead()) {
			c->tickAnimation(dT);
			window.draw(*c);
		}
	}
}


std::shared_ptr<LobbyInterface> GameClient::createNewLevel(int nLevels)
{
	lobby = std::make_shared<LobbyInterface>();
	lobby->requestLoadLevel("default_level.csv");
	saveLevelName = "USER_LEVEL_" + std::to_string(nLevels) + ".csv";
	return lobby;
}


void GameClient::setSaveName(std::string fileName) {
	saveLevelName = fileName;
}


const Settings& GameClient::getSettings() const
{
	return settings;
}

//	ui.setCharacters(characters);
