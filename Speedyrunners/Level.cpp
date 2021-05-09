#include <iostream> 
#include <fstream>
#include "Level.h"
#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "utils.hpp"

// airport: "../assets/Content/tiles/tiles_airport.png"
Level::Level(const sf::RenderWindow& window) : Level("../assets/Content/tiles/tiles_black_editor.png", "../assets/Content/Backgrounds/ENV_Airport/sky_1280_720.png", window)
{	
}

void Level::drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const
{
	collidableTiles.drawTile(target, states, pos, tileNumber);
}

void Level::setTile(const sf::Vector2i& pos, const int tileNumber)
{
	collidableTiles.setTile(pos, tileNumber);
}

void Level::save(const std::string& f_name) const
{
	std::cout << "Saving " << f_name << "\n";
	std::string version = "0";
	std::ofstream file("../assets/levels/" + f_name);
	if (!(file << version << std::endl)) { // formato, #MAX, # nombre fichero
		std::cerr << "File <assets/levels/" + f_name + "> inaccesible\n";
		//exit(1);
	}
	// Save background:
	file << backgroundPath << std::endl;
	// Save checkpoints:
	file << checkpoints.size() << "\n";
	for (auto cp : checkpoints) {
		file << cp.getPos().x << " " << cp.getPos().y << " " << cp.getRadius() << " ";
	}
	file << "\n";
	// Save tilemap:
	file << collidableTiles << std::endl;
	
	std::cout << "Level saved\n";

}

void Level::saveDuplicateVertical(const std::string& f_name) const
{
	std::cout << "Saving " << f_name << "\n";
	std::string version = "0";
	std::ofstream file("../assets/levels/" + f_name);
	if (!(file << version << std::endl)) { // formato, #MAX, # nombre fichero
		std::cerr << "File <assets/levels/" + f_name + "> inaccesible\n";
		//exit(1);
	}
	// Save background:
	file << backgroundPath << std::endl;
	// Save checkpoints:
	file << checkpoints.size() << "\n";
	for (auto cp : checkpoints) {
		file << cp.getPos().x << " " << cp.getPos().y << " " << cp.getRadius() << " ";
	}
	file << "\n";
	// Save tilemap:
	file << collidableTiles.to_string_dup_vertical() << std::endl;

	std::cout << "Level saved\n";
}

void Level::load(const std::string& f_name, const sf::RenderWindow& window)
{
	std::cout << "Loading " << f_name << "\n";

	std::ifstream file("../assets/levels/" + f_name);
	int format;
	std::string s;
	if (!(file >> format)) {
		std::cerr << "File <assets/levels/" + f_name + "> inaccesible\n";
		exit(1);
	}
	if (format != 0) {
		std::cerr << "Unknown format\n";
		exit(1);
	}
	std::string line;
	file >> backgroundPath;
	background.load(backgroundPath, window);
	file.ignore(); // skip \n

	// Load checkpoints:
	
	int nCheckpoints;
	file >> nCheckpoints;
	file.ignore();
	for (int i = 0; i < nCheckpoints; i++) {
		std::string sx, sy, sr;
		file >> sx >> sy >> sr;
		float x = std::stof(sx), y = std::stof(sy), r = std::stof(sr);
		checkpoints.emplace_back(sf::Vector2f(x, y), r);
	}
	file.ignore();
	
	// Load tilemap:
	if (!collidableTiles.load(file)) {
		std::cerr << "Error loading tilemap\n";
	}
	else {
		std::cout << "Level loaded\n";
	}
}


Level::Level(const std::string& tilesetPath, const std::string& bgPath, const sf::RenderWindow& window) :
	backgroundPath(bgPath), background(bgPath,window)
{	// Adapted from: https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays
	// define the level with an array of tile indices
	const int lvlSize = 128 * 64;
	int level[lvlSize];/* =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 5, 6, 7, 4, 1, 1, 1,
	};*/
	//RNG rng;
	//rng.fillRand(level, lvlSize, 0, 9);
	for (int i = 0; i < lvlSize; i++) {
		level[i] = 0; // All air
	}
	if (!collidableTiles.load(tilesetPath, sf::Vector2u(16, 16), level, 128, 64)) {
		std::cerr << "Error abriendo tileset " << tilesetPath << std::endl;
		//exit(1);
	}
	
	//loadBackground(bgPath, window);
}

void Level::loadBackground(const std::string& file, const sf::RenderWindow& window)
{
	background.load(file, window);
}


std::string Level::getBackgroundPath() const
{
	return backgroundPath;
}
const TileMap& Level::getCollidableTiles() const
{
	return collidableTiles;
}
void Level::setCheckpoints(const std::vector<Checkpoint>& cps)
{
	checkpoints = cps;
}
/*
void Level::scaleBackground(sf::RenderTarget& target) {
	//auto size = target.getSize();
	auto size = target.getView().getSize();
	
}*/

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//background.draw(target, states);
	target.draw(background);
	target.draw(collidableTiles, states);
	/*
	auto v = target.getView();
	auto s = states;
	target.setView(target.getDefaultView()); // background shouldnt be moved
	// apply the tileset texture
	states.texture = &bgTexture;
	states.transform = sf::Transform();
	// draw the vertex array
	target.draw(bgVertices, states);
	target.setView(v);
	target.draw(collidableTiles, s);
	*/
}



void Level::addCheckpoint(const sf::Vector2f& pos, float r)
{
	checkpoints.emplace_back(pos, r);
}

void Level::getCheckpoints(std::vector<Checkpoint>& _checkpoints) const
{
	_checkpoints = checkpoints;
}
