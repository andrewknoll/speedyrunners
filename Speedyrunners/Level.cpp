#include <iostream> 
#include <fstream>
#include "Level.h"
#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "utils.hpp"
#include "Resources.h"
#include "AudioPlayer.h"

// airport: "../assets/Content/tiles/tiles_airport.png"
Level::Level() : Level("../assets/Content/tiles/tiles_black_editor.png", "../assets/Content/Backgrounds/ENV_Airport/sky_1280_720.png")
{	
}

void Level::setInitialPosition(const sf::Vector2f& ini)
{
	initialPosition = ini;
}

sf::Vector2f Level::getInitialPosition() const
{
	return initialPosition;
}

void Level::addBoostBox(const sf::Vector2f& pos)
{
	boostBoxes.emplace_back(pos, collidableTiles.getTileSizeWorld().x);
}

bool Level::insideBoostbox(const sf::Vector2f& pos) const
{
	for (const auto& b : boostBoxes)
		if (b.isInside(pos)) return true;
	return false;
}

void Level::addItemPickup(const sf::Vector2f& pos)
{
	itemPickups.emplace_back(pos, collidableTiles.getTileSizeWorld().x);
}

void Level::addBoxObstacle(const sf::Vector2f& pos)
{
	boxObstacles.emplace_back(pos, collidableTiles.getTileSizeWorld().x);
}

// drop it, affected by gravity
void Level::dropCrate(const sf::Vector2f& pos) {
	boxObstacles.emplace_back(pos, collidableTiles.getTileSizeWorld().x, false).fallToFloor();
}

bool Level::checkItemPickups(std::shared_ptr<Character> character)
{
	if (character->getCurrentItem() == glb::item::NONE) { // Only check if the character doesnt already have an item
		const auto& hb = character->getHitBox();
		for (auto& i : itemPickups)
			if (i.isInside(hb)) { // Collides with a pickup
				audioPlayer.play(AudioPlayer::Effect::PICKUP);
				character->setItem(i.getItem()); // sets the item and puts the pickup in cd
				return true;
			}
	}
	return false;
}


bool Level::checkBoxCollisions(std::shared_ptr<Character> character)
{
	const auto& hb = character->getHitBox();
	bool hit = false; 
	int idx = 0;
	for (auto& i : boxObstacles) {
		if (i.isInside(hb)) { // Collides with a box
			audioPlayer.play(AudioPlayer::Effect::CRATE_DROP); // Not sure
			character->tumbleWithBox();
			hit = true;
			break;
		}
		idx++;
	}
	if (hit && !boxObstacles[idx].respawnable()) // Remove the box if needed
		boxObstacles.erase(boxObstacles.begin() + idx); 
	return hit;
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
		std::cerr << "File <../assets/levels/" + f_name + "> inaccesible\n";
		//exit(1);
	}
	// Save background:
	file << backgroundPath << std::endl;
	// Save initial position:
	file << initialPosition << "\n";
	// boost things:
	file << boostBoxes.size() << "\n";
	for (const auto& b : boostBoxes) {
		file << b.getPosition() << " ";
	}
	file << "\n";
	// item pickups things:
	file << itemPickups.size() << "\n";
	for (const auto& i : itemPickups) {
		file << i.getPosition() << " ";
	}
	file << "\n";
	// box obstacles:
	file << boxObstacles.size() << "\n";
	for (const auto& i : boxObstacles) {
		file << i.getPosition() << " ";
	}
	file << "\n";
	// Save checkpoints:
	file << checkpoints.size() << "\n";
	for (auto cp : checkpoints) {
		file << cp.getPos().x << " " << cp.getPos().y << " " << cp.getRadius() << " ";
	}
	file << "\n";
	// Save tilemap:
	file << collidableTiles << std::endl;

	std::ifstream lvls("../assets/levels/levels.csv");
	std::set<std::string> lvlsSet;
	std::string line;
	while (std::getline(lvls, line)) lvlsSet.insert(line);
	std::string name = f_name.substr(0, f_name.size() - 4);
	//std::cout << name << "\n";
	if (lvlsSet.find(name) == lvlsSet.end()) { // Doesnt contain it already
		std::ofstream lvlsList("../assets/levels/levels.csv", std::ios_base::app);
		lvlsList << name << "\n";
	}
	
	
	std::cout << "Level saved\n";

}

void Level::saveDuplicateVertical(const std::string& f_name) const
{
	std::cout << "Saving " << f_name << "\n";
	std::string version = "0";
	std::ofstream file("../assets/levels/" + f_name);
	if (!(file << version << std::endl)) { // formato, #MAX, # nombre fichero
		std::cerr << "File <../assets/levels/" + f_name + "> inaccesible\n";
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

void Level::load(const std::string& f_name)
{
	checkpoints.clear();
	boostBoxes.clear();
	itemPickups.clear();
	boxObstacles.clear();
	std::cout << "Loading " << f_name << "\n";

	std::ifstream file("../assets/levels/" + f_name);
	int format;
	std::string s;
	//if (!(file >> format)) {
	if (!(file >> s)) {

		std::cerr << "File <../assets/levels/" + f_name + "> inaccesible " << s << "\n";
		exit(1);
	}
	else {
		std::cout << "format: " << s << "\n";
		format = std::stoi(s);
	}
	if (format != 0) {
		std::cerr << "Unknown format\n";
		exit(1);
	}
	file.ignore(); // skip \n
	std::string line;
	file >> backgroundPath;
	// random thing:
	if (backgroundPath == "random") background = Background(rng::bgSampler.getSample()); // random background
	else background = Background(backgroundPath);
	file.ignore(); // skip \n

	// Ini pos:
	float x, y;
	file >> x >> y;
	file.ignore(); // skip \n
	initialPosition = sf::Vector2f(x, y);

	// boost things:
	int nBoosts;
	file >> nBoosts;
	std::vector<sf::Vector2f> boostPositions;
	for (int i = 0; i < nBoosts; i++) {
		file >> x >> y;
		boostPositions.emplace_back(x, y);
	}
	file.ignore();

	// item things:
	int nItems;
	file >> nItems;
	std::vector<sf::Vector2f> itemPositions;
	for (int i = 0; i < nItems; i++) {
		file >> x >> y;
		itemPositions.emplace_back(x, y);
	}
	file.ignore();

	// item things:
	int nBoxes;
	file >> nBoxes;
	std::vector<sf::Vector2f> boxPositions;
	for (int i = 0; i < nBoxes; i++) {
		file >> x >> y;
		boxPositions.emplace_back(x, y);
	}
	file.ignore();
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
	//std::cout << "loaded " << checkpoints.size() << " from file\n";
	
	// Load tilemap:
	if (!collidableTiles.load(file)) {
		std::cerr << "Error loading tilemap\n";
	}
	else {
		float tileW = collidableTiles.getTileSizeWorld().x;
		for (const auto& p : boostPositions) boostBoxes.emplace_back(p, tileW);
		for (const auto& p : itemPositions) itemPickups.emplace_back(p, tileW);
		for (const auto& p : boxPositions) boxObstacles.emplace_back(p, tileW);
		std::cout << "Level loaded\n";
	}
}


Level::Level(const std::string& tilesetPath, const std::string& bgPath) :
	backgroundPath(bgPath), background(bgPath), audioPlayer(Resources::getInstance().getAudioPlayer())
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

void Level::loadBackground(const std::string& file)
{
	background = Background(file);
}


void Level::setDefaultLevel()
{
	int width = 512;//collidableTiles.getWidth();
	int height = 256; //collidableTiles.getHeight();

	collidableTiles.setWidth(width);
	collidableTiles.setHeight(height);
	std::cout << "width, height: " << width << ", " << height << "\n";
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			//std::cout << i << " " << j << " " << collidableTiles.getSize() << "\n";
			if (i < height / 20 || i > 19 * height / 20 ||
				j < width / 20 || j > 19 * width / 20)
			{ // Floor borders
				collidableTiles.setTileIndexed(j, i, 1);
			}
			else { // Rest, Air
				collidableTiles.setTileIndexed(j, i, 0);
			}
		}
	}
}

std::string Level::getBackgroundPath() const
{
	return backgroundPath;
}
Background Level::getBackground() const
{
	return background;
}
const TileMap& Level::getCollidableTiles() const
{
	return collidableTiles;
}
void Level::setCheckpoints(const std::vector<Checkpoint>& cps)
{
	checkpoints = cps;
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(collidableTiles, states);
	for (const auto& b : boostBoxes)   target.draw(b, states);
	for (const auto& b : boxObstacles) target.draw(b, states);
	for (const auto& i : itemPickups)  target.draw(i, states);
}



void Level::addCheckpoint(const sf::Vector2f& pos, float r)
{
	checkpoints.emplace_back(pos, r);
}

void Level::getCheckpoints(std::vector<Checkpoint>& _checkpoints) const
{
	_checkpoints = checkpoints;
}

void Level::testBoxCollision(const sf::Vector2f& p)
{
	auto& box = boxObstacles.front();
	box.setPositionCheckTiles(p, collidableTiles);
}

void Level::update(const sf::Time& dT)
{
	for (auto& i : itemPickups)  i.update(dT);
	for (auto& b : boostBoxes)	 b.update(dT); // animations
	for (auto& b : boxObstacles) b.update(dT, collidableTiles); // animations
}
