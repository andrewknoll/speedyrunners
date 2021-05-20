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
    size_t i = pos.x / tileSizeWorld.x;
    size_t j = pos.y / tileSizeWorld.y;
    setTileIndexed(i, j, tileNumber);
    
}

Tiles::Collidable TileMap::getTile(int i, int j) const {
	return (Tiles::Collidable)tiles.at(j * width + i);
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


Tiles::Ramp Tiles::toRamp(Collidable tile)
{
    using namespace Tiles;
    if (tile == Collidable::RAMP_DOWN || tile == Collidable::STAIRS_DOWN) {
        return DOWN;
    }
    else if (tile == Collidable::RAMP_UP || tile == Collidable::STAIRS_UP) {
        return UP;
    }
    else if (tile == Collidable::RAMP_CEIL_DOWN) {
        return CEIL_DOWN;
    }
    else if (tile == Collidable::RAMP_CEIL_UP) {
        return CEIL_UP;
    }
    else {
        return Ramp::NONE;
    }
}

bool Tiles::isRamp(Collidable tile)
{
    return toRamp(tile) != Tiles::Ramp::NONE;
}



/*if (true || hDif > 0) {
                // Collision
                sf::Vector2f point = downR; // not exact
                sf::Vector2f n = geometry::normalize(sf::Vector2f(-1, -1));
                std::cout << "collided with ramp\n";
                return Tiles::Collision{ point, n, hDif/sqrt(2.0f)/2.0f }; // hDif is actually vertical distance, not normal one
            }*/
#ifdef OLD_RAMP_COLLISION
std::optional<Tiles::Collision> Tiles::rampCollision(const Tiles::Ramp ramp, const sf::Vector2f& tilePos,
    const sf::Vector2f& tileSize, const sf::FloatRect& hitbox) 
{   
    std::cout << "colliding ramp: " << ramp << "\n";
    if (ramp == Tiles::Ramp::UP) {
        // Ramp up
        sf::Vector2f downR(hitbox.left+hitbox.width, hitbox.top+hitbox.height); // down right point of the hitbox
        float dist;
        // Collision
        sf::Vector2f point = downR; // not exact
        sf::Vector2f n = geometry::normalize(sf::Vector2f(0, -1));
        if (downR.x < tilePos.x + tileSize.x) {
            float hDif = (downR.y - (tilePos.y - (downR.x - tilePos.x))); // diff in height
            std::cout << "hdif: " << hDif << "\n";
            
            //std::cout << "collided with ramp\n";
             // hDif is actually vertical distance, not normal one
            dist = hDif;
        }
        else { // No collision
            dist = downR.y - tilePos.y;
        }
        return Tiles::Collision{ point, n, dist };
    }
    else {
        // TODO: los otros 4 tipos de rampas
        return {};
    }
}
#else
std::optional<Tiles::Collision> Tiles::rampCollision(const Tiles::Ramp ramp, const sf::Vector2f& tilePos,
    const sf::Vector2f& tileSize, const sf::FloatRect& hitbox, bool isGrounded)
{
    //std::cout << "colliding ramp: " << ramp << "\n";
    if (ramp == Tiles::Ramp::UP || ramp == Tiles::Ramp::DOWN) {

        // Ramp up
        sf::Vector2f downCenter(hitbox.left + hitbox.width / 2.0f, hitbox.top + hitbox.height); // down center point of the hitbox
        float dist = 0;
        sf::Vector2f n = geometry::normalize(sf::Vector2f(0, -1));
        if (downCenter.x > tilePos.x && downCenter.x < tilePos.x + tileSize.x) {
            if (ramp == Tiles::Ramp::DOWN) downCenter.x = tilePos.x + tileSize.x - (downCenter.x - tilePos.x); // flip horizontally, the rest is the same
            sf::Vector2f target = downCenter;
            target.y = tilePos.y + tileSize.y - (downCenter.x - tilePos.x);
            dist = target.y - downCenter.y;//(downCenter.y - (tilePos.y  + (downCenter.x - tilePos.x - tileSize.y))); // diff in height
            /*std::cout << downCenter.x - tilePos.x << "\n-tilepos.y = " << -tilePos.y + (downCenter.x - tilePos.x)
               << "\n+downCenter.y = " << dist;*/
        }
        else return {};
        /*else if (downCenter.x > tilePos.x + tileSize.x) { // No collision
            dist = downCenter.y - tilePos.y;
            //dist = 0;
        }*/

        //sf::sleep(sf::seconds(2));
        if (!isGrounded && dist > 0) return {};
        return Tiles::Collision{ downCenter, n, -dist };
    }
    else {
        // TODO: los otros 4 tipos de rampas

        return {};
    }
}
#endif

bool Tiles::hasPriority(const Collision& a, const Collision& b)
{
    if (isRamp(a.tileType) && !isRamp(b.tileType)) return true;
    else if (!isRamp(a.tileType) && isRamp(b.tileType)) return false;
    return a.distance > b.distance;
}
// Adapted from the SAT method in https://laptrinhx.com/custom-physics-engine-part-2-manifold-generation-716517698/
std::optional<Tiles::Collision> Tiles::collision(const Tiles::Collidable tile, const sf::Vector2f& tilePos,
    const sf::Vector2f& tileSize, const sf::FloatRect& hitbox, bool isGrounded)
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
        /*if (x_overlap <= 0) std::cout << "no x overlap\n";
        else std::cout << "no y overlap\n";*/
        return {};
    } // Here we have a collision:
    // if its a ramp:
    if (Tiles::toRamp(tile) != NONE) return rampCollision(Tiles::toRamp(tile), tilePos, tileSize, hitbox);
    // Otherwise, its a square tile:
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

    return Tiles::Collision{ point, n, dist };// TODO: This is wrong!
}

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