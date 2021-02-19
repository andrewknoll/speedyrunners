#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>

inline float randInRange(const float min, const float max) {
	return min + static_cast<float>(max * rand() / RAND_MAX);
}


class Star {
	float x, y, z; // z profundidad
	sf::CircleShape shape;

	float map(float val, float min1, float max1, float min2, float max2) {
		return (max2 - min2) * (val - min1) / (max1 - min1) + min2;
	}

public:
	Star(const sf::RenderWindow& window) 
	: x(randInRange(0, window.getSize().x)), y(randInRange(0, window.getSize().y)), z(randInRange(0,window.getSize().x)),
		shape(5)
	{
		shape.setFillColor(sf::Color::Yellow);
	}

	void draw(sf::RenderWindow& window) {
		shape.setPosition(map(x / z, 0, 1, 0, window.getSize().x), map(y / z, 0, 1, 0, window.getSize().y));
		window.draw(shape);
		//sf::CircleShape shape(20.f);
	//shape.setFillColor(sf::Color::Red);
	}

	void update(const sf::RenderWindow& window) {
		z -= 1;
		if (z < 0) { // Si llega a la camara, se resetea 
			x = randInRange(0, window.getSize().x*0.96);
			y = randInRange(0, window.getSize().y*0.96);
		}
		//x -= (x / z) * window.getSize().x;
		//y -= (y / z) * window.getSize().y;
		//shape.setScale()
	}

};


inline void programaEstrellas()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "Stars");
	int N_ESTRELLAS = 100;
	std::vector<Star> stars;
	for (size_t i = 0; i < N_ESTRELLAS; i++)
	{
		stars.emplace_back(Star(window));
	}


	window.setFramerateLimit(60); //60 FPS


	while (window.isOpen())
	{

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

		for (auto star : stars) {
			star.update(window);
		}

		window.clear();
		for (auto star : stars) {
			star.draw(window);
		}
		window.display();
	}

}