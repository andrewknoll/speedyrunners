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
	size0 = getSize();
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

void Camera::follow(std::vector<CharPtr> characters, int first) {
	sf::Vector2f avg = sf::Vector2f(0.0, 0.0);
	sf::Vector2f distance = sf::Vector2f(0.0, 0.0);
	sf::Vector2f firstPos = characters[first]->getPosition();
	sf::FloatRect viewport = getViewport();
	viewport.width = getSize().x;
	viewport.height = getSize().y;

	//Calculate average position of all players
	for (int i = 0; i < characters.size(); i++) {
		avg += characters[i]->getPosition();
	}
	avg /= (float)characters.size();

	for (int i = 0; i < characters.size(); i++) {
		distance += avg - characters[i]->getPosition();
	}
	setSize(size0*((distance.x * distance.x + distance.y + distance.y) * glb::cameraZoomFunctionSteepness + 1.0f));
	
	//Ensure first player is inside bounds
	avg.x -= std::min(0.0f, firstPos.x - (viewport.left + viewport.width * glb::viewMarginFactor));
	avg.x -= std::max(0.0f, firstPos.x - (viewport.left + viewport.width * (1.0f - glb::viewMarginFactor)));
	avg.y -= std::min(0.0f, firstPos.y - (viewport.top + viewport.height * glb::viewMarginFactor));
	avg.y -= std::max(0.0f, firstPos.y - (viewport.top + viewport.height * (1.0f - glb::viewMarginFactor)));
	
	//Ensure camera doesn't go out of bounds
	avg.x = std::max(avg.x, viewport.width / 2.0f);
	avg.y = std::max(avg.y, viewport.height / 2.0f);

	setCenter(avg);
}