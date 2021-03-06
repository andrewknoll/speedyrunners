#pragma once


class Camera
{
	sf::View view;
public:
	Camera();
	Camera(const sf::FloatRect& rect);
	//sf::Rect<float> view;

	void setView(sf::RenderWindow& window) const;

};