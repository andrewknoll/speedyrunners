
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"

int main() {
	Game game;
	Spritesheet sprsht;
	sprsht.parse_spritesheet("../assets/Content/Characters/Falcon/animation_variant01.png", "../assets/indexes/Characters/Falcon/animation_atlas_variant00.json");
	Character falcon;//"../assets/Content/Characters/Falcon/");
	falcon.getSpritesVectorFromMap(sprsht.get_animations());
	falcon.setPosition(100, 100);
	//falcon.setScale(0.5, 0.5);
	Player me;
	me.setCharacter(&falcon);
	game.playerJoin(me);
	game.addCharacter(falcon);
	game.loop();
}