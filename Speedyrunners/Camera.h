#pragma once
#include "Character.h"
#include "SFML/Graphics.hpp"


class Camera : public sf::View
{
	using CharPtr = std::shared_ptr<Character>;
	//sf::View view;

private:
	const float EPSILON = 0.01f;
	float rectSizeFactor = 1.0f;
	
	bool dragging = false;
	bool suddenDeath = false;
	bool increasingRedness = true;
	
	sf::RectangleShape viewportShape;
	
	sf::Uint16 redValue = 0;

	sf::Vector2i lastMousePos;
	sf::Vector2f size0;
	sf::Vector2f objectivePos, objectiveSize;

public:
	Camera(const sf::FloatRect& rect = sf::FloatRect(0,0,10,10));
	//sf::Rect<float> view;

	void moveByMouse(sf::Vector2i pos);

	void resetDrag();
	
	void operator= (const sf::View& v);

	void follow(std::vector<CharPtr>& characters, int first = 0);

	sf::RectangleShape getSuddenDeathRectangle();

	sf::FloatRect viewRectangle() const;

	bool isInAllowedBounds(CharPtr character) const;

	void setSuddenDeath(bool sd);
	void update(sf::Time dT);
};