#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "utils.hpp"


Camera::Camera(const sf::FloatRect& rect) : sf::View(rect)
{
	// Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
	//view.reset(sf::FloatRect(100, 100, 400, 200));
	// Rotate it by 45 degrees
	//view.rotate(45);
	// Set its target viewport to be half of the window
	//view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
	// Apply it
	//window.setView(view);
	// Render stuff
	//window.draw(someSprite);
	// Set the default view back
	//window.setView(window.getDefaultView());
	// Render stuff not affected by the view
	//window.draw(someText);
}

void Camera::moveByMouse(sf::Vector2i pos)
{
	if (!dragging) {
		dragging = true;
	}
	else {

		sf::Vector2f dif = utils::toVector2f(lastMousePos - pos);

		move(dif);
	}
	lastMousePos = pos;
}

void Camera::resetDrag()
{
	dragging = false;
}

void Camera::operator=(const sf::View& v)
{
	setCenter(v.getCenter());
	setRotation(v.getRotation());
	setSize(v.getSize());

}












