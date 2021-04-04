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
    //tileSizeWorld = tileSize;
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
    quad[0].position = sf::Vector2f(i * tileSizeWorld.x, j * tileSizeWorld.y);
    quad[1].position = sf::Vector2f((i + 1) * tileSizeWorld.x, j * tileSizeWorld.y);
    quad[2].position = sf::Vector2f((i + 1) * tileSizeWorld.x, (j + 1) * tileSizeWorld.y);
    quad[3].position = sf::Vector2f(i * tileSizeWorld.x, (j + 1) * tileSizeWorld.y);

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
    size_t i = pos.x / tileSizeWorld.x;
    size_t j = pos.y / tileSizeWorld.y;
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
    int i = pos.x / tileSizeWorld.x;
    int j = pos.y / tileSizeWorld.y;
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
    str += tileSetPath + "\n" + std::to_string(tileSize.x) + " " + std::to_string(tileSize.y) 
        + " " + std::to_string(tileSizeWorld.x) + " " + std::to_string(tileSizeWorld.y) + "\n";
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
    std::string xstr, ystr;
    file >> tileSize.x >> tileSize.y >> xstr >> ystr;
    tileSizeWorld.x = stof(xstr);
    tileSizeWorld.y = stof(ystr);
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

std::vector<physics::Collision> TileMap::collision(const sf::FloatRect& characterHitbox) const
{
    // Tile coordinates of upper left tile:
    int i = int(characterHitbox.left) / tileSizeWorld.x;
    int j = int(characterHitbox.top) / tileSizeWorld.y;
    std::vector<physics::Collision> collisions;
	for (int dj = 0; dj < 3; dj++) { // And 3 vertical
        //bool bothHorizontal = false; // Both horizontal tiles are collidable
		for (int di = 0; di < 2; di++) { // Check the 2 horizontal tiles
            sf::Vector2f posRectTile = sf::Vector2f((i+di) * tileSizeWorld.x, (j+dj) * tileSizeWorld.y);

            sf::Vector2f sizeRectTile(tileSizeWorld.x, tileSizeWorld.y);
            if (tiles[(i + di) + (j + dj) * width] != 0) { // Tile isnt air 
                auto c = Tiles::collision(Tiles::FLOOR, posRectTile, sizeRectTile, characterHitbox);
                if (c) {
                    collisions.emplace_back(*c);
                    //std::cout << "Collided with tile " << di << " " << dj << "... " << i + di << " " << j+dj << "\n";
                    //return c; // collided, we return it, otherwise check the rest
                }
            }
        }
    }
    std::sort(collisions.begin(), collisions.end(),
        [](physics::Collision a, physics::Collision b) {
            return (a.distance > b.distance);
        }
    );
    return collisions;
}


/*std::optional<physics::Collision> Tiles::collision(const Tiles::Collidable tile, const sf::Vector2f& tilePos, const sf::Vector2f& tileSize, const sf::FloatRect& hitbox)
{
    sf::FloatRect tileRect(tilePos.x, tilePos.y, tileSize.x, tileSize.y);
    if (hitbox.intersects(tileRect)) {
        return physics::Collision{ sf::Vector2f() };
    }
    return {}; // No intersection
}*/


// Adapted from the SAT method in https://laptrinhx.com/custom-physics-engine-part-2-manifold-generation-716517698/
std::optional<physics::Collision> Tiles::collision(const Tiles::Collidable tile, const sf::Vector2f& tilePos, const sf::Vector2f& tileSize, const sf::FloatRect& hitbox)
{
    sf::FloatRect tileRect(tilePos.x, tilePos.y, tileSize.x, tileSize.y);
    // x overlap:
    float a_extent = (tileRect.width) / 2;
    float b_extent = (hitbox.width) / 2;
    float x_overlap = a_extent + b_extent - std::abs(hitbox.left+b_extent-(tileRect.left+a_extent)); // std::abs(hitbox.left-tileRect.left);
    // y overlap:
    a_extent = (tileRect.height) / 2;
    b_extent = (hitbox.height) / 2;
    float y_overlap = a_extent + b_extent - std::abs(hitbox.top+ b_extent - (tileRect.top+a_extent));
    if (x_overlap <= 0 || y_overlap <= 0) { // No overlap, no collision
        return {};
    } // Here we have a collision:
    // collision points:
    sf::Vector2f collision1(std::max(tileRect.left, hitbox.left), std::max(tileRect.top, hitbox.top));
    sf::Vector2f collision2(std::min(tileRect.left + tileRect.width, hitbox.left + hitbox.width),
                            std::min(tileRect.top + tileRect.height, hitbox.top + hitbox.height) );
  
    
    sf::Vector2f n; // Normal
    sf::Vector2f point; // Point
    float dist; // Distance
    if (x_overlap < y_overlap)
    {
        // Point towards B knowing that t points from A to B
        n = (hitbox.left - tileRect.left) < 0 ? sf::Vector2f(-1, 0) : sf::Vector2f(1, 0);
        dist = x_overlap;
        point = collision1; // TODO: This is wrong! -> we have to return both points i think
    }
    else
    {
        // Point toward B knowing that t points from A to B
        n = (hitbox.top - tileRect.top) < 0 ? sf::Vector2f(0, -1) : sf::Vector2f(0, 1);
        dist = y_overlap;
        point = collision2;
    }
    //std::cout << x_overlap << " " << y_overlap << "\n";

    return physics::Collision{ point, n, dist };// TODO: This is wrong!
}
