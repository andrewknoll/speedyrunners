
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Spritesheet.h"

int main(int argc, char** argv) {
	std::cout << argv[0] << std::endl;
	Spritesheet test;
	test.parse_spritesheet("../assets/Content/Characters/Speedrunner/animation_variant00.png", "../assets/indexes/Characters/Speedrunner/animation_atlas_variant00.json");
	
	sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Demo Game");
	sf::Event event;
	
	sf::Clock clock;

	Animation test_anim = test.get_animations()["Double-Jump"];
	sf::Sprite test_sprite = test_anim.get_first_frame();

	while (renderWindow.isOpen()) {
		while (renderWindow.pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed)
				renderWindow.close();
		}
		if (clock.getElapsedTime().asMilliseconds() > 100/3.0) {
			test_anim.advance_frame(test_sprite);
			std::cout << test_anim.get_current_frame() << std::endl;
			clock.restart();
		}

		renderWindow.clear();
		renderWindow.draw(test_sprite);
		renderWindow.display();
	}
	return 0;
}
