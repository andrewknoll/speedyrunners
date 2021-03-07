#include <iostream>
#include "TileMap.h"


void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &tileset;

    // draw the vertex array
    target.draw(vertices, states);
}

bool TileMap::load(const std::string& tileSetPath, sf::Vector2u _tileSize, const int* tiles, const int _width, const int _height)
{
    width = _width; height = _height;
    // load the tileset texture
    if (!tileset.loadFromFile(tileSetPath))
        return false;
    tileSize = _tileSize;
    // resize the vertex array to fit the level size
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &vertices[(i + j * width) * 4];
            setQuad(quad, i, j, tu, tv);
        }

    return true;
}

void TileMap::setQuad(sf::Vertex* quad, const int i, const int j, const int tu, const int tv) const {

    // define its 4 corners
    quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
    quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
    quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
    quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

    // define its 4 texture coordinates
    quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
    quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
    quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
    quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
}

void TileMap::setTile(const sf::Vector2i& pos, const int tileNumber) {

    // find its position in the tileset texture
    int tu = tileNumber % (tileset.getSize().x / tileSize.x);
    int tv = tileNumber / (tileset.getSize().x / tileSize.x);

    // define its 4 corners
    int i = pos.x / tileSize.x;
    int j = pos.y / tileSize.y;
    //std::cout << "drawing tile at " << pos.x << " " << pos.y << " " << tileNumber << std::endl;
    if ((i + j * width) >= width * height) {
        std::cerr << "Tile outside of array!!\n";
    }
    else {
        // get a pointer to the current tile's quad
        sf::Vertex* quad = &vertices[(i + j * width) * 4];
        setQuad(quad, i, j, tu, tv);
    }
}


void TileMap::drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const
{
    // find its position in the tileset texture
    int tu = tileNumber % (tileset.getSize().x / tileSize.x);
    int tv = tileNumber / (tileset.getSize().x / tileSize.x);

    sf::VertexArray tileVertices;
    tileVertices.setPrimitiveType(sf::Quads);
    tileVertices.resize(4);
    // Create the quad
    sf::Vertex* quad = &tileVertices[0];// = &vertices[(i + j * width) * 4];

    // define its 4 corners
    int i = pos.x / tileSize.x;
    int j = pos.y / tileSize.y;
    //std::cout << "drawing tile at " << pos.x << " " << pos.y << " " << tileNumber << std::endl;

    //std::cout << i << " " << j << std::endl;
    setQuad(quad, i, j, tu, tv);

    // apply the tileset texture
    states.texture = &tileset;
    //states.transform *= getTransform();

    // draw the vertex array
    target.draw(tileVertices, states);
}

std::string TileMap::to_string() const {
    std::string str = std::to_string(width) + " " + std::to_string(height) + "\n";
    /*for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j){

        }
    }*/
    return str;
}


std::ostream& operator<<(std::ostream& os, const TileMap& t) {
    os << t.to_string();
    return os;
}
