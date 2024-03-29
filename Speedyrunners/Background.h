#pragma once
#include <string>
#include "SFML/Graphics.hpp"


class Background : public sf::Drawable
{
	sf::Texture bgTexture;
	sf::VertexArray bgVertices;


	sf::Vector2f currentTexSize;



	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Background(const std::string& file, const sf::RenderWindow& window);
	Background(const std::string& file, const sf::RenderWindow& window, const sf::FloatRect& zona);
	void load(const std::string& file, const sf::RenderWindow& window);
	// zona especifica el rectangulo de la pantalla que ocupa el fondo (las 4 esquinas de 0 a 1)
 	void load(const std::string& file, const sf::RenderWindow& window, const sf::FloatRect& zona);

	void setTextureCoords(const sf::FloatRect& zona);

	// Changes the height of the background to respect the original texture's width/height proportions
	void fixProportions();
	
};

