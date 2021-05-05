#pragma once
#include "Character.h"
#include "SFML/Graphics.hpp"


class Camera : public sf::View
{
	using CharPtr = std::shared_ptr<Character>;
	//sf::View view;
	sf::Vector2i lastMousePos; 
	bool dragging = false;
	sf::Vector2f size0;
	const float EPSILON = 0.01f;
	sf::RectangleShape viewportShape;
	bool suddenDeath = false;
	float rectSizeFactor = 1.0f;
	sf::Uint16 redValue = 0;
	bool increasingRedness = true;
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
};