#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>

inline float randInRange(const float min, const float max) {
	return min + static_cast<float>((max-min) * rand() / RAND_MAX);
}


class Star {
	float x, y, z; // z profundidad
	sf::CircleShape shape;

	float map(float val, float min1, float max1, float min2, float max2) {
		return (val - min1) * (max2 - min2) / (max1 - min1) + min2;
	}

public:
	Star(const sf::RenderWindow& window) 
	:	x(randInRange(-(window.getSize().x / 2.0), window.getSize().x/2.0)), 
		y(randInRange(-(window.getSize().y / 2.0), window.getSize().y/2.0)),
		z(randInRange(0.5, 2)*window.getSize().x),
		shape(1)
	{
		shape.setFillColor(sf::Color::White);
	}

	void draw(sf::RenderWindow& window) {//window.getSize().x / 2.0 +
		float shapex =  map(x / z, -1, 1, 0, window.getSize().x);
		float shapey =  map(y / z, -1, 1, 0, window.getSize().y);
		shape.setPosition(shapex, shapey);
		window.draw(shape);
		//sf::CircleShape shape(20.f);
	//shape.setFillColor(sf::Color::Red);
	}

	void update(const sf::RenderWindow& window) {
		z -= 15;
		if (z <= 0) { // Si llega a la camara, se resetea 
			x = randInRange(-(window.getSize().x / 2.0), window.getSize().x / 2.0);
			y = randInRange(-(window.getSize().y / 2.0), window.getSize().y / 2.0);
			z = randInRange(0.5, 2) * window.getSize().x;
		}
		//x -= (x / z) * window.getSize().x;
		//y -= (y / z) * window.getSize().y;
		float scale = window.getSize().x*2/z/5;
		shape.setScale(scale, scale);
	}

};


inline void programaEstrellas()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "Stars");
	int N_ESTRELLAS = 1000;
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


		window.clear();
		for (auto& star : stars) {
			star.update(window); // update pos
			star.draw(window); // draw
		}
		window.display();
	}

}