#pragma once

#include <string>
#include "TileMap.h"
#include "Checkpoint.h"
#include "Background.h"


class Camera; // Pre-declaracion mejor que include en el hpp


//template <std::size_t rows, std::size_t cols>
class Level : public sf::Drawable
{
	TileMap collidableTiles;
	TileMap decorationTiles;

	// Background:
	Background background;
	//sf::Texture bgTexture;
	//sf::Sprite background;
	//sf::VertexArray bgVertices;


	std::string backgroundPath;
	//int rows, cols;
	//std::vector<std::vector<TileMap>> tileMap;
	//std::vector<sf::Texture> spriteSheets; // se deben mantener en memoria: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Sprite.php

	std::vector<Checkpoint> checkpoints;



	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Level(const std::string& tilesetPath, const std::string& bgPath);


	void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const;

	Level(const sf::RenderWindow& window);

	void setTile(const sf::Vector2i& pos, const int tileNumber);


	void save(const std::string& f_name) const;

	void saveDuplicateVertical(const std::string& f_name) const;

	void load(const std::string& f_name, const sf::RenderWindow& window);

	Level(const std::string& tilesetPath, const std::string& bgPath, const sf::RenderWindow& window);

	void loadBackground(const std::string& file, const sf::RenderWindow& window);


	std::string getBackgroundPath() const;

	const TileMap& getCollidableTiles() const;

	void setCheckpoints(const std::vector<Checkpoint>& cps);

	void addCheckpoint(const sf::Vector2f& pos, float r);

	void getCheckpoints(std::vector<Checkpoint>& _checkpoints) const;
	//void draw(sf::RenderWindow& window, const Camera& cam) const;

};

