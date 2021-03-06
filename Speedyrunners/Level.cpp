#include <iostream> 
#include "Level.h"
#include "SFML/Graphics.hpp"
#include "Camera.h"


Level::Level() : Level("../assets/Content/tiles/tiles_airport.png", "../assets/Content/Backgrounds/ENV_Airport/sky_1280_720.png")
{

	
}


Level::Level(const std::string& tilesetPath, const std::string& bgPath)
{	// Adapted from: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays
	// define the level with an array of tile indices
	const int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 5, 6, 7, 4, 1, 1, 1,
	};

	if (!collidableTiles.load(tilesetPath, sf::Vector2u(32, 32), level, 16, 8)) {
		std::cerr << "Error abriendo tileset " << tilesetPath << std::endl;
		//exit(1);
	}

	if (!bgTexture.loadFromFile(bgPath))
	{	// TODO: parametro del fondo
		std::cerr << "Error cargando fondo\n";
	}
	background.setTexture(bgTexture);
}


void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(collidableTiles, states);
}
