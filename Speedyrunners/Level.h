#pragma once

#include <string>
#include "TileMap.h"
#include "Checkpoint.h"
#include "Background.h"
#include "BoostBox.h"
#include "ItemPickup.h"
#include "BoxObstacle.h"


class Camera; // Pre-declaracion mejor que include en el hpp
class Character;
class AudioPlayer;

//template <std::size_t rows, std::size_t cols>
class Level : public sf::Drawable
{
	TileMap collidableTiles;
	TileMap decorationTiles;

	// Background:
	Background background;
	//sf::Texture bgTexture;
	//sf::Sprite background;
	//sf::VertexArray bgVertices;
	sf::Vector2f initialPosition = sf::Vector2f(200, 190);


	std::string backgroundPath;
	
	// Audio:
	AudioPlayer& audioPlayer;

	std::vector<Checkpoint> checkpoints;

	// Interactible thingies:
	std::vector<BoostBox> boostBoxes;
	std::vector<ItemPickup> itemPickups;
	std::vector<BoxObstacle> boxObstacles;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Level(const std::string& tilesetPath, const std::string& bgPath);

	void setInitialPosition(const sf::Vector2f &ini);

	sf::Vector2f getInitialPosition() const;

	void addBoostBox(const sf::Vector2f& pos);

	bool insideBoostbox(const sf::Vector2f& pos) const;

	void addItemPickup(const sf::Vector2f& pos);

	void addBoxObstacle(const sf::Vector2f& pos);

	void dropCrate(const sf::Vector2f& pos);

	bool checkItemPickups(std::shared_ptr<Character> character);
	
	bool checkBoxCollisions(std::shared_ptr<Character> character);

	void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const;

	Level();

	void setTile(const sf::Vector2i& pos, const int tileNumber);


	void save(const std::string& f_name) const;

	void saveDuplicateVertical(const std::string& f_name) const;

	void load(const std::string& f_name);

	void loadBackground(const std::string& file);


	void setDefaultLevel();

	std::string getBackgroundPath() const;

	Background getBackground() const;

	const TileMap& getCollidableTiles() const;

	void setCheckpoints(const std::vector<Checkpoint>& cps);

	void addCheckpoint(const sf::Vector2f& pos, float r);

	void getCheckpoints(std::vector<Checkpoint>& _checkpoints) const;

	void testBoxCollision(const sf::Vector2f& p);

	void update(const sf::Time& dT);
	//void draw(sf::RenderWindow& window, const Camera& cam) const;


	Level(Level const&) = default;

	Level& operator=(Level other);
};

