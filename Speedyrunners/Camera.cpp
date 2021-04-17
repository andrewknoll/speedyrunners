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
	float avgDistance = 0.0f;
	sf::Vector2f firstPos = characters[first]->getPosition();
	sf::FloatRect viewport = getViewport();
	viewport.width = getSize().x;
	viewport.height = getSize().y;

	//Calculate average position of all players
	for (int i = 0; i < characters.size(); i++) {
		avg += characters[i]->getPosition();
	}
	avg /= (float)characters.size();
	
	//Ensure first player is inside bounds
	avg.x -= std::min(0.0f, firstPos.x - (avg.x - viewport.width /2.0f * glb::viewMarginFactor));
	avg.x -= std::max(0.0f, firstPos.x - (avg.x + viewport.width /2.0f * glb::viewMarginFactor));
	avg.y += std::min(0.0f, firstPos.y - (avg.y - viewport.height /2.0f * glb::viewMarginFactor));
	avg.y += std::max(0.0f, firstPos.y - (avg.y + viewport.height /2.0f* glb::viewMarginFactor));

	//Calculate scale, depending on the average of the distance of each character to the center
	for (int i = 0; i < characters.size(); i++) {
		distance = avg - characters[i]->getPosition();
		avgDistance += std::sqrtf(distance.x * distance.x + distance.y + distance.y);
	}
	avgDistance /= characters.size();

	setSize(size0*(avgDistance * glb::cameraZoomFunctionSteepness + 1.0f));
	
	//Ensure camera doesn't go out of bounds
	avg.x = std::max(avg.x, viewport.width / 2.0f);
	avg.y = std::max(avg.y, viewport.height / 2.0f);

	setCenter(avg);
}