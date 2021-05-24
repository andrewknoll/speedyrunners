#pragma warning( disable : 4244 ) 

#include <iostream>
#include <fstream>
#include "TileMap.h"
#include "utils.hpp"


void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &tileset;

    // draw the vertex array
    target.draw(vertices, states);
}

size_t TileMap::getSize() const {
    return tiles.size();
}

void TileMap::resize() {

    tiles.resize(width * height);
	meta_tiles.resize(width * height);

    vertices.resize(width * height * 4);
}

bool TileMap::load(const std::string& _tileSetPath, sf::Vector2u _tileSize, const int* _tiles, const size_t _width, const size_t _height)
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
    resize();

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


void TileMap::setTileIndexed(size_t row, size_t col, const int tileNumber) {
    int i = row, j = col;

    //std::cout << "drawing tile at " << pos.x << " " << pos.y << " " << tileNumber << std::endl;
    if ((i + j * width) >= width * height) {
        std::cerr << "Tile outside of array!!\n";
    }
    else {
        tiles[i + j * width] = tileNumber;
        // find its position in the tileset texture
        int tu = tileNumber % (tileset.getSize().x / tileSize.x);
        int tv = tileNumber / (tileset.getSize().x / tileSize.x);

        // get a pointer to the current tile's quad
        sf::Vertex* quad = &vertices[(i + j * width) * 4];
        setQuad(quad, i, j, tu, tv);
    }
}

void TileMap::setTile(const sf::Vector2i& pos, const int tileNumber) {
    // Get row and col:
    size_t i = std::ceilf(0.5f + pos.x / tileSizeWorld.x);
    size_t j = std::ceilf(0.5f + pos.y / tileSizeWorld.y);
    setTileIndexed(i, j, tileNumber);
    
}

void TileMap::setMetaTile(const sf::Vector2i & pos, std::shared_ptr<MetaTile> mt) {
	size_t i = pos.x / tileSizeWorld.x;
	size_t j = pos.y / tileSizeWorld.y;
	if ((i + j * width) >= width * height) {
		std::cerr << "Tile outside of array!!\n";
	}
	else {
		meta_tiles[i + j * width] = mt;
	}
}

Tiles::Collidable TileMap::getTile(int i, int j) const {
	return (Tiles::Collidable)tiles.at(j * width + i);
}

std::shared_ptr<MetaTile> TileMap::getMetaTile(int i, int j) const {
	return meta_tiles.at(j * width + i);
}

size_t TileMap::getWidth() const {
	return width;
}

size_t TileMap::getHeight() const {
	return height;
}

void TileMap::setWidth(size_t newWidth) {
    width = newWidth;
    resize();
}

void TileMap::setHeight(size_t newHeight) {
    height = newHeight;
    resize();
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

std::string TileMap::to_string(bool duplicarHorizontal) const {
    std::string str = "";
    if (duplicarHorizontal)
        str += std::to_string(2 * width) + " " + std::to_string(height) + "\n";
    else
        str += std::to_string(width) + " " + std::to_string(height) + "\n";
    
    //std::string str = std::to_string(width) + " " + std::to_string(height) + "\n";
    str += tileSetPath + "\n" + std::to_string(tileSize.x) + " " + std::to_string(tileSize.y) 
        + " " + std::to_string(tileSizeWorld.x) + " " + std::to_string(tileSizeWorld.y) + "\n";
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j){
            str += std::to_string(tiles[j + i * width]) + " ";
        }
        /* duplicar mapa */
        if (duplicarHorizontal) {
            for (size_t j = 0; j < width; ++j)
                str += std::to_string(tiles[width - j - 1 + i * width]) + " ";
        }
        /**************************/
        str += "\n";
    }
    
    return str;
}

std::string TileMap::to_string_dup_vertical() const
{
    std::string str = "";
    
    str += std::to_string(width/2) + " " + std::to_string(height*2) + "\n";
    //std::string str = std::to_string(width) + " " + std::to_string(height) + "\n";
    str += tileSetPath + "\n" + std::to_string(tileSize.x) + " " + std::to_string(tileSize.y)
        + " " + std::to_string(tileSizeWorld.x) + " " + std::to_string(tileSizeWorld.y) + "\n";
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width/2; ++j) {
            str += std::to_string(tiles[j + i * width]) + " ";
        }
        /**************************/
        str += "\n";
    }
    for (size_t di = 0; di < height; ++di) {
        size_t i = di;
        for (size_t dj = 0; dj < width / 2; ++dj) {
            size_t j = dj + width / 2 + 1;
            str += std::to_string(tiles[j + i * width]) + " ";
        }
        /**************************/
        str += "\n";
    }

    return str;
}


bool TileMap::load(std::ifstream& file) {
    file >> width >> height;
    vertices.setPrimitiveType(sf::Quads);
    resize();
    file.ignore();
    file >> tileSetPath;
    file.ignore();
    file >> tileSize.x >> tileSize.y >> tileSizeWorld.x >> tileSizeWorld.y;
    file.ignore();

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            file >> tiles[j + i * width];
        }
        file.ignore(); // \n
    }
    if (!tileset.loadFromFile(tileSetPath))
        return false;


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

std::vector<struct Tiles::Collision> TileMap::collision(const sf::FloatRect& characterHitbox, bool isGrounded) const
{
    // Tile coordinates of upper left tile:
    int i = int(characterHitbox.left) / tileSizeWorld.x;
    int j = int(characterHitbox.top) / tileSizeWorld.y;
    std::vector<struct Tiles::Collision> collisions;
    
    int nVertical = 1 + std::roundf(characterHitbox.height / tileSizeWorld.y);
    int nHorizontal = 1 + std::roundf(characterHitbox.width / tileSizeWorld.x);


    sf::Vector2f sizeRectTile(tileSizeWorld.x, tileSizeWorld.y);
    //std::cout << "sizeTile: " << sizeRectTile << " size hitbox: " << characterHitbox.width << " " << characterHitbox.height << "\n";
    //if (nVertical < 3) std::cout << "checking " << nHorizontal << " horizontal and " << nVertical << " vertical tiles\n";
    
	for (int dj = 0; dj < nVertical; dj++) { // And 3 vertical
        //bool bothHorizontal = false; // Both horizontal tiles are collidable
		for (int di = 0; di < nHorizontal; di++) { // Check the 2 horizontal tiles
            sf::Vector2f posRectTile = sf::Vector2f((i+di) * tileSizeWorld.x, (j+dj) * tileSizeWorld.y);
            int idx = (i + di) + (j + dj) * width;
            if (idx >= tiles.size()) continue; // check bounds
            
			auto tile = tiles[idx];
            //std::cout << "idx: " << idx << " ";
            if (tile != 0) { // Tile isnt air 
                auto c = Tiles::collision((Tiles::Collidable)tile, posRectTile, sizeRectTile, characterHitbox, isGrounded);
                if (c) {
                    //std::cout << "yes ";
                    c->tileType = (Tiles::Collidable)tile;
                    collisions.emplace_back(*c);
                    //std::cout << "Collided with tile " << di << " " << dj << "... " << i + di << " " << j+dj << "\n";
                    //return c; // collided, we return it, otherwise check the rest
                }
                //else std::cout << "no ";

            }
            //else std::cout << "is air ";
            //std::cout << "\n";
        }
    }
    std::sort(collisions.begin(), collisions.end(), Tiles::hasPriority);
        /*[](struct Tiles::Collision& a, struct Tiles::Collision& b) {
            return ((isRamp(a.tileType) && !isRamp(b.tileType)) 
                 || (std::abs(a.distance) > std::abs(b.distance)));
        }*/
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

std::vector<Tiles::Collidable> TileMap::tilesToTheSide(const sf::FloatRect& characterHitbox, bool rightSide, float checkWidth) const
{
    std::vector<Tiles::Collidable> side;

    // Tile coordinates of upper left tile:
    int i, j;
    float widthChecked = characterHitbox.width * checkWidth;
    if (rightSide) {
        i = int(characterHitbox.left + characterHitbox.width) / tileSizeWorld.x;
        j = int(characterHitbox.top) / tileSizeWorld.y;
    }
    else {
        i = int(characterHitbox.left - widthChecked) / tileSizeWorld.x;
        j = int(characterHitbox.top) / tileSizeWorld.y;
    }

    int nVertical = std::max(1, int(std::round(characterHitbox.height / tileSizeWorld.y)));
    int nHorizontal = 1 + std::roundf(widthChecked / tileSizeWorld.x);


    sf::Vector2f sizeRectTile(tileSizeWorld.x, tileSizeWorld.y);
    //std::cout << "sizeTile: " << sizeRectTile << " size hitbox: " << characterHitbox.width << " " << characterHitbox.height << "\n";
    //std::cout << "checking " << nHorizontal << " horizontal and " << nVertical << " vertical tiles\n";

    for (int dj = 0; dj < nVertical; dj++) { // And 3 vertical
        //bool bothHorizontal = false; // Both horizontal tiles are collidable
        for (int di = 0; di < nHorizontal; di++) { // Check the 2 horizontal tiles
            int idx = (i + di) + (j + dj) * width;
            if (idx >= tiles.size()) continue; // check bounds

            auto tile = tiles[idx];
            side.push_back((Tiles::Collidable)tile);
        }
    }
    return side;
}

sf::Vector2u TileMap::getTileSizeWorld() const{
	return tileSizeWorld;
}