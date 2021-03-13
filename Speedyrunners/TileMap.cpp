#pragma warning( disable : 4244 ) 

#include <iostream>
#include <fstream>
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

bool TileMap::load(const std::string& _tileSetPath, sf::Vector2u _tileSize, const int* _tiles, const int _width, const int _height)
{
    width = _width; height = _height;
    // load the tileset texture
    tileSetPath = _tileSetPath;
    tileSize = _tileSize;
    if (!tileset.loadFromFile(_tileSetPath))
        return false;
    tileSize = _tileSize;
    // resize the vertex array to fit the level size
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (size_t i = 0; i < width; ++i)
        for (size_t j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = _tiles[i + j * width];
            tiles[i + j * width] = tileNumber;
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
    size_t i = pos.x / tileSize.x;
    size_t j = pos.y / tileSize.y;
    tiles[i + j * width] = tileNumber;
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
    str += tileSetPath + "\n" + std::to_string(tileSize.x) + " " + std::to_string(tileSize.y) + "\n";
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j){
            str += std::to_string(tiles[j + i * width]) + " ";
        }
        str += "\n";
    }
    return str;
}


bool TileMap::load(std::ifstream& file) {
    file >> width >> height;
    file.ignore();
    file >> tileSetPath;
    file.ignore();
    file >> tileSize.x >> tileSize.y;
    file.ignore();

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            file >> tiles[j + i * width];
        }
        file.ignore(); // \n
    }
    if (!tileset.loadFromFile(tileSetPath))
        return false;

    // resize the vertex array to fit the level size
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (size_t i = 0; i < width; ++i)
        for (size_t j = 0; j < height; ++j)
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


std::ostream& operator<<(std::ostream& os, const TileMap& t) {
    os << t.to_string();
    return os;
}

std::optional<physics::Collision> TileMap::collision(const sf::FloatRect& characterHitbox) const
{
    // Tile coordinates of upper left tile:
    int i = int(characterHitbox.left) / tileSize.x;
    int j = int(characterHitbox.top) / tileSize.y;
    for (int di = 0; di < 2; di++) { // Check the 2 horizontal tiles
        for (int dj = 0; dj < 3; dj++) { // And 3 vertical
            sf::Vector2f posRectTile = sf::Vector2f((i+di) * tileSize.x, (j+dj) * tileSize.y);

            sf::Vector2f sizeRectTile(tileSize.x, tileSize.y);
            if (tiles[(i + di) + (j + dj) * width] != 0) { // Tile isnt air 
                auto c = Tiles::collision(Tiles::FLOOR, posRectTile, sizeRectTile, characterHitbox);
                if (c) {
                    std::cout << "Collided with tile " << di << " " << dj << "... " << i + di << " " << j+dj << "\n";
                    return c; // collided, we return it, otherwise check the rest
                }
            }
        }
    }
    
    return {};
}


std::optional<physics::Collision> Tiles::collision(const Tiles::Collidable tile, const sf::Vector2f& tilePos, const sf::Vector2f& tileSize, const sf::FloatRect& hitbox)
{
    sf::FloatRect tileRect(tilePos.x, tilePos.y, tileSize.x, tileSize.y);
    if (hitbox.intersects(tileRect)) {
        return physics::Collision{ sf::Vector2f() };
    }
    return {}; // No intersection
}
