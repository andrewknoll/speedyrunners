#pragma once
#include "Character.h"


class Camera : public sf::View
{
	using CharPtr = std::shared_ptr<Character>;
	//sf::View view;
	sf::Vector2i lastMousePos; 
	bool dragging = false;
	sf::Vector2f size0;
public:
	Camera(const sf::FloatRect& rect = sf::FloatRect(0,0,10,10));
	//sf::Rect<float> view;

	void moveByMouse(sf::Vector2i pos);

	void resetDrag();
	
	void operator= (const sf::View& v);

	void follow(std::vector<CharPtr> characters, int first = 0);
};