
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"
#include "NPC.h"
#include "MusicPlayer.h"
#include "Resources.h"

int main() {

	Game game;
	Resources& src = Resources::getInstance();

	game.loopMenu();

	//game.defaultInit(2);
	
	game.loop();
}
