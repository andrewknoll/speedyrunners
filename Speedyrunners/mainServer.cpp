
#include "SFML/Graphics.hpp"
#include "GameServer.h"
#include "Spritesheet.h"
#include "NPC.h"
#include "MusicPlayer.h"
#include "Resources.h"
#include "SFML/Network.hpp"

void rngTest() {
	auto& sam = rng::itemSampler;
	int rockets = 0; int TESTS = 10000;
	for (int i = 0; i < TESTS; i++) {
		if (sam.sample() == glb::ROCKET) rockets++;
	}
	std::cout << rockets << " rockets out of " << TESTS << " tests\n";
}

int main() {
	
	GameServer game;
	Resources& src = Resources::getInstance();

	game.loop();
}
