
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"

int main() {
	Game game;
	Spritesheet sprsht;
	sprsht.parse_spritesheet("../assets/Content/Characters/Speedrunner/animation_variant01.png", "../assets/indexes/Characters/Speedrunner/animation_atlas_variant00.json");
	Character falcon(sprsht); //"../assets/Content/Characters/Falcon/");
	falcon.setPosition(200, 200);
	//falcon.setScale(0.5, 0.5);
	Player me;
	auto falconPtr = std::make_shared<Character>(falcon);
	me.setCharacter(falconPtr);
	game.playerJoin(me);
	game.addCharacter(falconPtr);
	game.loop();
}