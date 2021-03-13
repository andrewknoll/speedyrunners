#pragma once
#include <array>
#include "SFML/Graphics.hpp"

// Adapted from: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays


const int MAX_TILEMAP_SIZE = 8192;//128*64

class TileMap : public sf::Transformable, public sf::Drawable
{
protected:
	std::array<int, MAX_TILEMAP_SIZE> tiles;

	sf::VertexArray vertices;
	sf::Texture tileset;
	sf::Vector2u tileSize; 
	std::string tileSetPath;


	int width, height;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
public:

	bool load(const std::string& _tileSetPath, sf::Vector2u _tileSize, const int* _tiles, const int _width, const int _height);
	// Loads the tilemap from a saved file (csv):
	bool load(std::ifstream& file);

	void setQuad(sf::Vertex* quad, const int i, const int j, const int tu, const int tv) const;

	void setTile(const sf::Vector2i& pos, const int tileNumber);

	void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const;

	std::string to_string() const;


	//void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2u& pos, const int tileNumber) const;

	//void drawTile(const sf::Vector2u& pos, const sf::Vector2u& idx, const int type) const;
};

std::ostream& operator<<(std::ostream& os, const TileMap& t);


namespace Tiles {
	//const int FLOOR = 0;
	enum Collidable { // En tiles_black_editor.png
		AIR,
		FLOOR,
		JUMP_WALL_L,
		GRAPPLABLE,
		JUMP_WALL_R,
		WEIRD_SQUARES, // No se que es esto
		RAMP_UP,
		RAMP_DOWN,
		STAIRS_UP,
		STAIRS_DOWN,
		WEIRD_RAMP_UP, // Estas dos tampoco
		WEIRD_RAMP_DOWN, // "    "
		RAMP_CEIL_DOWN,
		RAMP_CEIL_UP,
		WEIRD_RAMP_CEIL_DOWN,
		WEIRD_RAMP_CEIL_UP
	};

	const int NB_COLLIDABLE = 16;
}