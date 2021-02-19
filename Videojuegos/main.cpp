
#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
	sf::CircleShape shape(20.f);
	shape.setFillColor(sf::Color::Red);

	window.setFramerateLimit(60); //60 FPS

	srand(time(NULL));

	float randX = static_cast<float>(5.0 * rand()/RAND_MAX), randY = static_cast<float>(5.0 * rand()/RAND_MAX);

	while (window.isOpen())
	{
		if (shape.getPosition().x + shape.getRadius()*2 + randX > window.getSize().x
			|| shape.getPosition().x < 0) {
			randX = -randX;
			
		}
		if (shape.getPosition().y + shape.getRadius() * 2 + randY > window.getSize().y
			|| shape.getPosition().y < 0) {
			randY = -randY;
		}
		
		shape.move(randX, randY);
		
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}