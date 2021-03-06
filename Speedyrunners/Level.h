#pragma once

#include <string>
#include "TileMap.h"


class Camera; // Pre-declaracion mejor que include en el hpp


//template <std::size_t rows, std::size_t cols>
class Level : public sf::Drawable
{
	TileMap collidableTiles;
	TileMap decorationTiles;
	//int rows, cols;
	//std::vector<std::vector<TileMap>> tileMap;
	//std::vector<sf::Texture> spriteSheets; // se deben mantener en memoria: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Sprite.php


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Level(const std::string& file);

	//Level();

	Level(const int rows = 50, const int cols = 50);

	//void draw(sf::RenderWindow& window, const Camera& cam) const;

};

