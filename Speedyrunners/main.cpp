
#include "SFML/Graphics.hpp"
#include "Game.h"

int main() {
	Game game;
	Character falcon;//"../assets/Content/Characters/Falcon/");
	falcon.setPosition(30, 30);
	//falcon.setScale(0.5, 0.5);
	game.addCharacter(falcon);
	game.loop();
	
	return 0;
}
