#pragma once

#include <string>
#include "TileMap.h"
#include "Checkpoint.h"
#include "Background.h"
#include "BoostBox.h"
#include "ItemPickup.h"


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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Level(const std::string& tilesetPath, const std::string& bgPath);

	void setInitialPosition(const sf::Vector2f &ini);

	sf::Vector2f getInitialPosition() const;

	void addBoostBox(const sf::Vector2f& pos);

	bool insideBoostbox(const sf::Vector2f& pos) const;

	void addItemPickup(const sf::Vector2f& pos);

	bool checkItemPickups(std::shared_ptr<Character> character);

	void drawTile(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2i& pos, const int tileNumber) const;

	Level(const sf::RenderWindow& window);

	void setTile(const sf::Vector2i& pos, const int tileNumber);


	void save(const std::string& f_name) const;

	void saveDuplicateVertical(const std::string& f_name) const;

	void load(const std::string& f_name, const sf::RenderWindow& window);

	Level(const std::string& tilesetPath, const std::string& bgPath, const sf::RenderWindow& window);

	void loadBackground(const std::string& file, const sf::RenderWindow& window);


	void setDefaultLevel();

	std::string getBackgroundPath() const;

	const TileMap& getCollidableTiles() const;

	void setCheckpoints(const std::vector<Checkpoint>& cps);

	void addCheckpoint(const sf::Vector2f& pos, float r);

	void getCheckpoints(std::vector<Checkpoint>& _checkpoints) const;


	void update(const sf::Time& dT);
	//void draw(sf::RenderWindow& window, const Camera& cam) const;

};

