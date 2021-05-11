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

	sf::Vector2u texSize;
	sf::Vector2u boostTexSize;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateBoostBar(int charidx, float proportion);

	void addPoints(sf::Vector2f pos, float widgetWidth);

	void addBoostBar(sf::Vector2f pos, float widgetWidth);


public:
	//InGameUI(std::vector<CharPtr> characters);
	InGameUI();
	InGameUI(sf::RenderWindow& _window);

	void setWindow(sf::RenderWindow& _window);



	void setCharacters(std::vector<CharPtr> characters);
	void update();
	sf::IntRect texRectFor(int point);
	void updatePoints();
};

