// Néstor Monzon y Andres Otero
// Campo de estrellas
// Solucion general basada en: https://www.youtube.com/watch?v=17WoOqgXsRM


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
	sf::Sprite sprite;
	float lastx, lasty;
	float rotationSpeed;
	//std::vector<sf::CircleShape> trail;

	float map(float val, float min1, float max1, float min2, float max2) {
		return (val - min1) * (max2 - min2) / (max1 - min1) + min2;
	}

public:
	Star(const sf::RenderWindow& window, const sf::Texture& tex)
	:	x(randInRange(-1,1)*window.getSize().x), 
		y(randInRange(-1,1)* window.getSize().y),
		z(randInRange(0.5, 2)*window.getSize().x),
		sprite(tex), lastx(x), lasty(y), rotationSpeed(randInRange(-15,15))
	{
		sprite.setOrigin(sprite.getLocalBounds().width/2.0f, sprite.getLocalBounds().height/ 2.0f);
		sprite.setScale(0.004, 0.004);
		sprite.setRotation(randInRange(0, 360));
		//sprite.setFillColor(sf::Color(255,255,200,255));
	}

	void draw(sf::RenderWindow& window) {//window.getSize().x / 2.0 +
		window.draw(sprite, sf::RenderStates(sf::BlendAlpha));
	}

	void update(const sf::RenderWindow& window, const float& speed=15) {
		z -= speed; 
		float shapex = map(x / z, -0.5, 0.5, 0, window.getSize().x);
		float shapey = map(y / z, -0.5, 0.5, 0, window.getSize().y);

		if (z <= 0 || shapex == 0 || shapex >= window.getSize().x 
			|| shapey == 0 || shapey >= window.getSize().y) { // Si se sale de la vista, se resetea 
			x = randInRange(-0.7, 0.7) * window.getSize().x;
			y = randInRange(-0.7, 0.7) * window.getSize().y;
			z = randInRange(0.5, 2) * window.getSize().x;
			shapex = map(x / z, -0.5, 0.5, 0, window.getSize().x);
			shapey = map(y / z, -0.5, 0.5, 0, window.getSize().y);
			rotationSpeed = randInRange(-15, 15);
		}
		sprite.setPosition(shapex, shapey);
		sprite.rotate(rotationSpeed);
		float scale = 0.05*std::max(0.f,0.4f*window.getSize().x/z); // 
		sprite.setScale(scale, scale);
	}

	float getZ() const {
		return z;
	}

	float getScale() const {
		return sprite.getScale().x;
	}

};


inline bool isCloser(const Star& s1, const Star& s2) {
	return s1.getZ() > s2.getZ();
}
inline bool isSmaller(const Star& s1, const Star& s2) {
	return s1.getScale() < s2.getScale();
}

inline void programaEstrellas()
{
	sf::Event event;
	float speed = 11;
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "Stars");
	int N_ESTRELLAS = 1400;
	std::vector<Star> stars;

	sf::Texture texture;
	texture.loadFromFile("assets/sprite-sprite-s.png");
	for (size_t i = 0; i < N_ESTRELLAS; i++)
	{
		stars.emplace_back(Star(window, texture));
	}
	/*std::sort(stars.begin(), stars.end(), isSmaller);
	for (auto s : stars) 
	{
		std::cout << s.getZ() << std::endl;
	}*/
	


	window.setFramerateLimit(60); //60 FPS

	// de https://en.sfml-dev.org/forums/index.php?topic=7018.0:
	float fps;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

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
		
		window.clear(sf::Color(0,0,0,128));//sf::Color(0,0,0,1));
		std::sort(stars.begin(), stars.end(), isSmaller);

		for (auto& star : stars) {
			star.update(window, speed); // update pos
			star.draw(window); // draw
		}
		window.display();


		// https://en.sfml-dev.org/forums/index.php?topic=7018.0:
		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
		if (int(currentTime.asMilliseconds())%100 < 2) // Para que no este sacandolo todos los frames
			std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
		previousTime = currentTime;

	}

}