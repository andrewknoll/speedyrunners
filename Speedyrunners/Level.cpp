#include <iostream> 
#include "Level.h"
#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "utils.hpp"

// airport: "../assets/Content/tiles/tiles_airport.png"
Level::Level() : Level("../assets/Content/tiles/tiles_black_editor.png", "../assets/Content/Backgrounds/ENV_Airport/sky_1280_720.png")
{

	
}

void Level::drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const
{
	collidableTiles.drawTile(target, states, pos, tileNumber);
}

void Level::setTile(const sf::Vector2i& pos, const int tileNumber)
{
	collidableTiles.setTile(pos, tileNumber);
}


Level::Level(const std::string& tilesetPath, const std::string& bgPath)
{	// Adapted from: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays
	// define the level with an array of tile indices
	const int lvlSize = 64 * 32;
	int level[lvlSize];/* =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 5, 6, 7, 4, 1, 1, 1,
	};*/
	//RNG rng;
	//rng.fillRand(level, lvlSize, 0, 9);
	for (int i = 0; i < lvlSize; i++) {
		level[i] = 0;
	}
	if (!collidableTiles.load(tilesetPath, sf::Vector2u(16, 16), level, 64, 32)) {
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
