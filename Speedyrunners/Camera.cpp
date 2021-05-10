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
	viewportShape = sf::RectangleShape(size0);
	viewportShape.setFillColor(sf::Color::Transparent);
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

void Camera::follow(std::vector<CharPtr>& characters, int first) {
	sf::Vector2f avg = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f newCenter = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f center, diff;
	sf::Vector2f distance = sf::Vector2f(0.0f, 0.0f);
	float avgDistance = 0.0f;
	sf::Vector2f firstPos = characters[first]->getPosition();
	sf::FloatRect viewport = viewRectangle();

	int count = 0;

	//Calculate average position of all players
	for (int i = 0; i < characters.size(); i++) {
		if (!characters[i]->isDead()) {
			avg += characters[i]->getPosition();
			count++;
		}
	}
	avg /= (float)count;
	
	//Ensure first player is inside bounds
	avg.x -= std::min(0.0f, firstPos.x - (avg.x - viewport.width /2.0f * glb::viewMarginFactor));
	avg.x += std::max(0.0f, firstPos.x - (avg.x + viewport.width /2.0f * glb::viewMarginFactor));
	avg.y += std::min(0.0f, firstPos.y - (avg.y - viewport.height /2.0f * glb::viewMarginFactor));
	avg.y -= std::max(0.0f, firstPos.y - (avg.y + viewport.height /2.0f* glb::viewMarginFactor));

	if (!suddenDeath) {
		//Calculate scale, depending on the average of the distance of each character to the center
		for (int i = 0; i < characters.size(); i++) {
			if (!characters[i]->isDead()) {
				distance = avg - characters[i]->getPosition();
				avgDistance += utils::length(distance);
			}
		}
		avgDistance /= count;
		objectiveSize = size0 * (avgDistance * glb::cameraZoomFunctionSteepness + 1.0f);
		//std::cout << "Size: " << size.x << " " << size.y << " avg: " << "\n";
	}
	//Ensure camera doesn't go out of bounds
	avg.x = std::max(avg.x, viewport.width / 2.0f);
	avg.y = std::max(avg.y, viewport.height / 2.0f);
	//TO-DO: Check max value

	objectivePos = avg;

	if (suddenDeath) {
		setSize(size0);
		if (rectSizeFactor > 0.3) {
			rectSizeFactor -= 1e-4f;
			viewportShape.setSize(size0 * rectSizeFactor);
		}
		viewportShape.setOutlineThickness(std::max(size0.x, size0.y) * (1.0f - rectSizeFactor) + 1.0f);
		viewportShape.setPosition(getCenter() - size0 * rectSizeFactor / 2.0f);
		if (increasingRedness) {
			redValue += 2;
		}
		else {
			redValue -= 2;
		}
		if (redValue < 2 || redValue > 253) {
			increasingRedness = !increasingRedness;
		}
		viewportShape.setOutlineColor(sf::Color(redValue, 0, 0, 255));
	}
}

sf::RectangleShape Camera::getSuddenDeathRectangle() {
	return viewportShape;
}

sf::FloatRect Camera::viewRectangle() const {
	sf::FloatRect viewport = sf::FloatRect();
	viewport.width = viewportShape.getSize().x;
	viewport.height = viewportShape.getSize().y;
	viewport.left = getCenter().x - viewport.width / 2;
	viewport.top = getCenter().y - viewport.height / 2;
	return viewport;
}

bool Camera::isInAllowedBounds(CharPtr character) const {
	auto pos = character->getPosition();
	return viewRectangle().contains(pos);
}

void Camera::setSuddenDeath(bool sd) {
	this->suddenDeath = sd;
}

void Camera::update(sf::Time dT) {
	sf::Vector2f movement = objectivePos - getCenter();
	sf::Vector2f sizeDiff = objectiveSize - getSize();
	move(movement * dT.asSeconds());
	if (!suddenDeath) {
		setSize(size0 + sizeDiff * dT.asSeconds());
		viewportShape.setSize(size0 + sizeDiff * dT.asSeconds());
	}
}
