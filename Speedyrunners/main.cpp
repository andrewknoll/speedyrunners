
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"

int main() {
	Game game;
	Spritesheet sprsht;
	sprsht.parse_spritesheet("../assets/Content/Characters/Speedrunner/animation_variant01.png", "../assets/indexes/Characters/Speedrunner/animation_atlas_variant00.json");
	std::shared_ptr<Character> speedyrunner = std::make_shared<Character>(sprsht); //"../assets/Content/Characters/Falcon/");
	speedyrunner->setPosition(200, 200);
	//falcon.setScale(0.5, 0.5);
	Player me;
	me.setCharacter(speedyrunner);
	game.playerJoin(me);
	speedyrunner->setName("Player");
	game.addCharacter(speedyrunner);
	game.loop();
}