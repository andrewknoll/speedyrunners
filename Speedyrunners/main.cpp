
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"
#include "NPC.h"
#include "MusicPlayer.h"
#include "Resources.h"

void rngTest() {
	auto& sam = rng::itemSampler;
	int rockets = 0; int TESTS = 10000;
	for (int i = 0; i < TESTS; i++) {
		if (sam.sample() == glb::ROCKET) rockets++;
	}
	std::cout << rockets << " rockets out of " << TESTS << " tests\n";
}

int main() {
	
	Game game;
	Resources& src = Resources::getInstance();

	game.loopMenu();

	//game.defaultInit(2);
	
	game.loop();
}
