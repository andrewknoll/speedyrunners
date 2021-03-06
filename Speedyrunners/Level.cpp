#include <iostream> 
#include "Level.h"
#include "SFML/Graphics.hpp"
#include "Camera.h"


Level::Level(const std::string& file)
{

	
}


Level::Level(const int rows, const int cols)
{
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
	std::string tileset = "../assets/Content/tiles/tiles_airport.png";

	if (!collidableTiles.load(tileset, sf::Vector2u(32, 32), level, 16, 8)) {
		std::cerr << "Error abriendo tileset " << tileset << std::endl;
		//exit(1);
	}

	/*sf::Texture airportTiles;
	airportTiles.loadFromFile("assets/Content/tiles/tiles-airport.png");
	spriteSheets.emplace_back(std::make_shared<sf::Texture>(airportTiles)); // para mantenerala en memoria
	for (size_t i = 0; i < rows; i++)
	{
		tileMap.emplace_back(std::vector<TileMap>(cols));
		for (size_t j = 0; j < cols; j++)
		{
			tileMap[i].emplace_back(TileMap(airportTiles, i%5));
		}
	}*/
	// TODO: Nivel por defecto, hardcodeado de momento supongo
}


void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(collidableTiles, states);
}
