#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "UISprite.h"

class Character;

class InGameUI : public sf::Drawable
{
	using CharPtr = std::shared_ptr<Character>;

	sf::RenderWindow* window;

	std::vector<std::unique_ptr<UISprite>> sprites;

	std::vector<CharPtr> chars;

	const sf::Texture& pointsTexture;
	std::vector<std::vector<sf::Sprite>> roundPoints;
	std::vector<std::vector<sf::Sprite>> boostSprites;
	std::vector<std::vector<sf::Sprite>> items;

	sf::Vector2u texSize;
	sf::Vector2u boostTexSize;
	sf::Vector2u itemTexSize;
	sf::IntRect bgItemTexRect;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateBoostBar(int charidx, float proportion);

	void updateItem(int charidx, glb::item item);


	void addPoints(sf::Vector2f pos, float widgetWidth);

	void addBoostBar(sf::Vector2f pos, float widgetWidth);

	void addItems(sf::Vector2f pos, float widgetWidth);
	sf::IntRect texRectFor(int point);

public:
	//InGameUI(std::vector<CharPtr> characters);
	InGameUI();
	InGameUI(sf::RenderWindow& _window);

	void setWindow(sf::RenderWindow& _window);



	

	void setCharacters(std::vector<CharPtr> characters);
	void update();
	void updatePoints();
};

