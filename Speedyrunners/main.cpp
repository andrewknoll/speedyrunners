
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"
#include "NPC.h"

int main() {
	Game game;
	Spritesheet sprsht;
	sprsht.parse_spritesheet("../assets/Content/Characters/Speedrunner/animation_variant01.png", "../assets/indexes/Characters/Speedrunner/animation_atlas_variant00.json");
	Spritesheet sprsht2;
	sprsht2.parse_spritesheet("../assets/Content/Characters/Cosmonaut/animation_variant01.png", "../assets/indexes/Characters/Cosmonaut/animation_atlas_variant00.json");
	std::shared_ptr<Character> speedyrunner = std::make_shared<Character>(sprsht); //"../assets/Content/Characters/Falcon/");
	speedyrunner->setPosition(200, 200);

	std::shared_ptr<Character> cosmonaut = std::make_shared<Character>(sprsht2); //"../assets/Content/Characters/Falcon/");
	cosmonaut->setPosition(200, 200);


	std::shared_ptr<Player> me = std::make_shared<Player>();
	me->setCharacter(speedyrunner);
	game.playerJoin(me);
	speedyrunner->setName("Player");
	game.addCharacter(speedyrunner);
	

	std::shared_ptr<Player> other = std::make_shared<NPC>();
	other->setCharacter(cosmonaut);
	game.playerJoin(other);
	cosmonaut->setName("NPC");
	game.addCharacter(cosmonaut);
	
	game.loop();
}