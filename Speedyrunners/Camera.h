#pragma once


class Camera
{
	sf::View view;
public:
	Camera(const sf::FloatRect& rect = sf::FloatRect(0,0,10,10));
	//sf::Rect<float> view;

	void setView(sf::RenderWindow& window) const;

};