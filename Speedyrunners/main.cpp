
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"

int main() {
	Game game;
	Character falcon;//"../assets/Content/Characters/Falcon/");

	falcon.setPosition(30, 130);
	//falcon.setScale(0.5, 0.5);
	game.addCharacter(falcon);
	//sf::sleep(sf::seconds(3));
	game.loop();
}