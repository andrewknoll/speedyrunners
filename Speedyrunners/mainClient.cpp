
#include "GameClient.h"
#include "SFML/Network.hpp"

int main() {

	GameClient client;

	client.loopMenu();

	client.loop();
}
