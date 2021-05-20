#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "utils.hpp"
#include "Resources.h"


void Camera::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(viewportShape, states);
}

Camera::Camera(const sf::FloatRect& rect) : sf::View(rect)
{
	size0 = getSize();
	viewportShape = sf::RectangleShape(size0);
	viewportShape.setFillColor(sf::Color::Transparent);
	viewportShape.setOrigin(sf::Vector2f(rect.width / 2.0f, rect.height / 2.0f));
	viewportShape.setPosition(getCenter()/*+ sf::Vector2f(rect.width / 2.0f, rect.height / 2.0f)*/);
	setSuddenDeath(false);
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

void Camera::immediateFollow(std::vector<CharPtr>& characters, int first) {
	follow(characters, first);
	setCenter(objectivePos);
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
	avg.x += std::min(0.0f, firstPos.x - (avg.x - viewport.width / 2.0f * glb::viewMarginFactor)); // Correct to the left
	avg.x += std::max(0.0f, firstPos.x - (avg.x + viewport.width / 2.0f * glb::viewMarginFactor)); // To the right
	avg.y += std::min(0.0f, firstPos.y - (avg.y - viewport.height / 2.0f * glb::viewMarginFactor)); // Up
	avg.y += std::max(0.0f, firstPos.y - (avg.y + viewport.height / 2.0f* glb::viewMarginFactor)); // Down

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
}

sf::FloatRect Camera::viewRectangle() const {
	sf::FloatRect viewport = sf::FloatRect();
	viewport.width = viewportShape.getSize().x * viewportShape.getScale().x;
	viewport.height = viewportShape.getSize().y * viewportShape.getScale().y;
	viewport.left = getCenter().x - viewport.width / 2;
	viewport.top = getCenter().y - viewport.height / 2;
	return viewport;
}

bool Camera::isInAllowedBounds(CharPtr character) const {
	auto pos = character->getPosition();
	return viewRectangle().contains(pos);
}

sf::Vector2f Camera::closestInView(const sf::Vector2f& p) const
{
	// TODO: esto no funciona!
	auto center = getCenter();
	auto direction = p - center;
	auto r = utils::length(getSize())/2.0f;
	std::cout << "center: " << center << "\n";
	return center;// +direction * r;
}



void Camera::setSuddenDeath(bool sd) {
	this->suddenDeath = sd;
}

void Camera::update(sf::Time dT) {
	sf::Vector2f movement = objectivePos - getCenter();
	sf::Vector2f sizeDiff = objectiveSize - getSize();
	move(movement * dT.asSeconds() * 5.0f);
	viewportShape.setOutlineThickness(std::max(size0.x, size0.y) * (1.0f - rectSizeFactor) + 1.0f);
	viewportShape.setOutlineColor(sf::Color(redValue, 0, 0, 255));
	if (suddenDeath) {
		setSize(size0);
		if (rectSizeFactor > 0.5) {
			rectSizeFactor -= 1e-4f;
			viewportShape.setScale(rectSizeFactor, rectSizeFactor);
		}
		if (increasingRedness) {
			redValue += 2;
		}
		else {
			redValue -= 2;
		}
		if (redValue < 2 || redValue > 253) {
			increasingRedness = !increasingRedness;
		}
	}
	else {
		if (rectSizeFactor < 1) {
			rectSizeFactor += 0.001;
			viewportShape.setScale(rectSizeFactor, rectSizeFactor);
		}
		else {
			if (rectSizeFactor > 1) {
				rectSizeFactor = 1.0f;
			}
			setSize(size0 + sizeDiff * dT.asSeconds());
		}
	}
}
