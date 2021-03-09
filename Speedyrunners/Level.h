#pragma once

#include <string>
#include "TileMap.h"


class Camera; // Pre-declaracion mejor que include en el hpp


//template <std::size_t rows, std::size_t cols>
class Level : public sf::Drawable
{
	TileMap collidableTiles;
	TileMap decorationTiles;

	// Background:
	sf::Texture bgTexture;
	sf::Sprite background;

	std::string backgroundPath;
	//int rows, cols;
	//std::vector<std::vector<TileMap>> tileMap;
	//std::vector<sf::Texture> spriteSheets; // se deben mantener en memoria: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Sprite.php


	void loadBackground(const std::string& file);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Level(const std::string& tilesetPath, const std::string& bgPath);

	Level();

	void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const;

	void setTile(const sf::Vector2i& pos, const int tileNumber);

	void save(const std::string& f_name) const;

	void load(const std::string& f_name);



	//void draw(sf::RenderWindow& window, const Camera& cam) const;

};

