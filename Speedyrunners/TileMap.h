#pragma once
#include "SFML/Graphics.hpp"

// Adapted from: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays


class TileMap : public sf::Transformable, public sf::Drawable
{

protected:
	//std::shared_ptr<sf::Texture> spriteSheet; // imagen con los tiles

	sf::VertexArray vertices;
	sf::Texture tileset;


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    

public:

	bool load(const std::string& tileSetPath, sf::Vector2u tileSize, const int* tiles, const int width, const int height);

};

