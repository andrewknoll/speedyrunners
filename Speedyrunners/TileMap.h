#pragma once
#include <array>
#include <optional>
#include "SFML/Graphics.hpp"
#include "utils.hpp"
#include "MetaTile.h"
#include "Tiles.h"


// Adapted from: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays


//const long MAX_TILEMAP_SIZE = 8192*8;//128*64

class TileMap : public sf::Transformable, public sf::Drawable
{
protected:
	//std::array<int, MAX_TILEMAP_SIZE> tiles;
	std::vector<int> tiles;
	std::vector<std::shared_ptr<MetaTile> > meta_tiles;

	sf::VertexArray vertices;
	sf::Texture tileset;
	sf::Vector2u tileSize;
	sf::Vector2u tileSizeWorld;
	std::string tileSetPath;



	size_t width, height;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    
public:

	size_t getSize() const;

	void resize();

	bool load(const std::string& _tileSetPath, sf::Vector2u _tileSize, const int* _tiles, const size_t _width, const size_t _height);
	// Loads the tilemap from a saved file (csv):
	bool load(std::ifstream& file);

	void setQuad(sf::Vertex* quad, const int i, const int j, const int tu, const int tv) const;

	void setTileIndexed(size_t row, size_t col, const int tileNumber);

	void setTile(const sf::Vector2i& pos, const int tileNumber);

	void setMetaTile(const sf::Vector2i& pos, std::shared_ptr<MetaTile> mt);

	Tiles::Collidable getTile(int i, int j) const;

	std::shared_ptr<MetaTile> getMetaTile(int i, int j) const;

	size_t getWidth() const;
	size_t getHeight() const;

	void setWidth(size_t newWidth);

	void setHeight(size_t newHeight);

	void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const;

	std::string to_string(bool duplicarHorizontal = false) const;
	std::string to_string_dup_vertical() const;

	std::vector<struct Tiles::Collision> collision(const sf::FloatRect& characterHitbox, bool isGrounded=false) const;

	// returns the tiles to the right or left of the hitbox (in a rectangle of 0.1*width of the hitbox)
	std::vector<Tiles::Collidable> tilesToTheSide(const sf::FloatRect& characterHitbox, bool rightSide=true, float checkWidth=0.1f) const;

	sf::Vector2u getTileSizeWorld() const;



	//void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2u& pos, const int tileNumber) const;

	//void drawTile(const sf::Vector2u& pos, const sf::Vector2u& idx, const int type) const;
};

std::ostream& operator<<(std::ostream& os, const TileMap& t);