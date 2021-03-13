#pragma once


class Camera : public sf::View
{
	//sf::View view;
	sf::Vector2i lastMousePos; 
	bool dragging = false;
public:
	Camera(const sf::FloatRect& rect = sf::FloatRect(0,0,10,10));
	//sf::Rect<float> view;

	void moveByMouse(sf::Vector2i pos);

	void resetDrag();
	

};