
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"
#include "NPC.h"
#include "MusicPlayer.h"
#include "Resources.h"

int main() {
	Game game;
	Resources& src = Resources::getInstance();

	std::shared_ptr<Character> speedyrunner = std::make_shared<Character>(src.getSpriteSheet(0)); //"../assets/Content/Characters/Falcon/");
	speedyrunner->setPosition(200, 200);

	std::shared_ptr<Character> cosmonaut = std::make_shared<Character>(src.getSpriteSheet(1)); //"../assets/Content/Characters/Falcon/");
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
	/* no estoy seguro de si esto tiene que estar,por si acaso lo dejo comentado
	game.music().loadMusicFile("../assets/Content/Audio/Music/Track 01 - Level.ogg", MusicPlayer::MusicType::REGULAR);
	game.music().loadMusicFile("../assets/Content/Audio/Music/Track02 - SD.ogg", MusicPlayer::MusicType::SUDDENDEATH);
	game.music().loadMusicFile("../assets/Content/Audio/Music/Track 03 - Menu.ogg", MusicPlayer::MusicType::MENU);
	*/
	game.loop();
}
