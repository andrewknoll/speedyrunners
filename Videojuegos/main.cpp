
#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>

#include "stars.hpp"


void fixPosition(const sf::RenderWindow& window, sf::CircleShape& shape) {
	// comprobar que la figura siga dentro:
	if (shape.getPosition().x + shape.getRadius() > window.getSize().x) {
		shape.setPosition(window.getSize().x - shape.getRadius() * 2, shape.getPosition().y);
	}
	if (shape.getPosition().y + shape.getRadius() > window.getSize().y) {
		shape.setPosition(shape.getPosition().x, window.getSize().y - shape.getRadius() * 2);
	}
}

void fixPosition(const sf::RenderWindow& window, sf::Sprite& sprite) {
	// comprobar que la figura siga dentro:
	if (sprite.getPosition().x + sprite.getGlobalBounds().width > window.getSize().x) {
		sprite.setPosition(window.getSize().x - sprite.getGlobalBounds().width, sprite.getPosition().y);
	}
	if (sprite.getPosition().y + sprite.getGlobalBounds().height > window.getSize().y) {
		sprite.setPosition(sprite.getPosition().x, window.getSize().y - sprite.getGlobalBounds().height);
	}
}



void programaSpriteRebota() {

	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	// D.E.P. CircleShape
	//sf::CircleShape shape(20.f);
	//shape.setFillColor(sf::Color::Red);


	// ------------ Sprite:
	// Declare and load a texture
	sf::Texture texture;
	texture.loadFromFile("assets/sprite-sprite-s.png");
	// Create a sprite
	sf::Sprite sprite(texture);// , sf::IntRect(0, 0, 200, 400));
	//sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition(100, 25);

	// Resize:
	sprite.scale(0.4, 0.4);

	// Draw it
	window.draw(sprite);



	window.setFramerateLimit(60); //60 FPS

	srand(time(NULL));

	float randX = static_cast<float>(20.0 * rand() / RAND_MAX), randY = static_cast<float>(20.0 * rand() / RAND_MAX);

	while (window.isOpen())
	{
		if (sprite.getPosition().x + sprite.getGlobalBounds().width + randX > window.getSize().x
			|| sprite.getPosition().x < 0) {
			randX = -randX;
		}
		if (sprite.getPosition().y + sprite.getGlobalBounds().height + randY > window.getSize().y
			|| sprite.getPosition().y < 0) {
			randY = -randY;
		}

		sprite.move(randX, randY);


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);

				window.setView(sf::View(visibleArea));

				fixPosition(window, sprite); // Recoloca la figura si es necesario
			}
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

}



int main()
{
	enum class Ejercicio{Sprite, Estrellas};
	Ejercicio ejSelec = Ejercicio::Estrellas;
	if (ejSelec == Ejercicio::Sprite) {
		programaSpriteRebota();
	}
	else {
		programaEstrellas();
	}
	return 0;
}