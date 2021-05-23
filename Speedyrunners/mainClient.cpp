
#include "GameClient.h"
#include "SFML/Network.hpp"
#include <filesystem>

int main() {

	GameClient client;
	client.loopMenu();

	client.loop();
}
