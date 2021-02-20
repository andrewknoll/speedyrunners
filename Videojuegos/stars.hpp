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
	:	x(randInRange(-1,1)*window.getSize().x), 
		y(randInRange(-1,1)* window.getSize().y),
		z(randInRange(0.5, 2)*window.getSize().x),
		shape(1)
	{
		shape.setFillColor(sf::Color(255,255,128,255));
	}

	void draw(sf::RenderWindow& window) {//window.getSize().x / 2.0 +
		float shapex =  map(x / z, -0.5, 0.5, 0, window.getSize().x);
		float shapey =  map(y / z, -0.5, 0.5, 0, window.getSize().y);
		shape.setPosition(shapex, shapey);
		window.draw(shape, sf::RenderStates(sf::BlendAlpha));
		//sf::CircleShape shape(20.f);
	//shape.setFillColor(sf::Color::Red);
	}

	void update(const sf::RenderWindow& window, const float& speed=15) {
		z -= speed;
		if (z <= 0) { // Si llega a la camara, se resetea 
			x = randInRange(-(window.getSize().x / 2.0), window.getSize().x / 2.0);
			y = randInRange(-(window.getSize().y / 2.0), window.getSize().y / 2.0);
			z = randInRange(0.5, 2) * window.getSize().x;
		}
		//x -= (x / z) * window.getSize().x;
		//y -= (y / z) * window.getSize().y;
		float scale = std::max(0.6f,window.getSize().x*2/z/5);
		shape.setScale(scale, scale);
	}

};


inline void programaEstrellas()
{
	sf::Event event;
	float speed = 15;
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

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				window.close();
			
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);

				window.setView(sf::View(visibleArea));
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { // accelerate
			speed *= 1.05;
			speed = std::min(speed, 120.f);
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) { // decelerate
			speed *= 0.95;
		}
		
		window.clear(sf::Color(10,10,10,128));//sf::Color(0,0,0,1));
		for (auto& star : stars) {
			star.update(window, speed); // update pos
			star.draw(window); // draw
		}
		window.display();
	}

}