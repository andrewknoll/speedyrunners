#include <iostream>
#include "UISprite.h"
#include "utils.hpp"
#include <cassert>
void UISprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (ignoreCamera) {
		auto view = target.getView();
		target.setView(target.getDefaultView());
		target.draw(sprite);
		target.setView(view);
	}
	else {
		target.draw(sprite);
	}
}

UISprite::UISprite(std::string texturePath)
{
	loadTexture(texturePath);
}

UISprite::UISprite(std::string texturePath, const Settings& settings,
	const float& relativeSize, const sf::Vector2f& relativePosition, bool clickable,
	const std::vector<sf::FloatRect>& relativeTextureRects) 
	: relTextureRects(relativeTextureRects)
{
	loadTexture(texturePath);
	ignoreCamera = true;
	// set the sprite size and position:
	auto resolution = settings.windowResolution();
	auto scale = resolution.x / sprite.getGlobalBounds().width * relativeSize;
	sprite.setScale(scale, scale);
	sprite.setPosition(resolution.x * relativePosition.x, resolution.y * relativePosition.y);

	if (relativeTextureRects.size() > 0) {
		setRelativeTextureRect(relativeTextureRects[0]);
	}
	
	if (clickable) makeClickable();
	std::cout << "placed sprite at " << sprite.getPosition() << " " << sprite.getScale() << "\n";
}

void UISprite::setRelativeTextureRect(const int idx) {
	if (idx < relTextureRects.size()) {
		auto rect = utils::relativeToGlobalTextureRect(relTextureRects[idx], texture);
		sprite.setTextureRect(rect);
	}
}


void UISprite::setRelativeTextureRect(const sf::FloatRect& relRect) {
	auto rect = utils::relativeToGlobalTextureRect(relRect, texture);
	sprite.setTextureRect(rect);
}

void UISprite::makeClickable() { 
	setClickableArea(sprite.getGlobalBounds()); 
}

void UISprite::loadTexture(std::string texturePath)
{
	try {
		if (!texture.loadFromFile(texturePath)) {
			std::cerr << "Error loading texture " << texturePath << "\n";
		}
		else {
			std::cout << texturePath << " texture loaded\n";
			sprite.setTexture(texture);
		}
	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}	
}

bool UISprite::mouseInside(const sf::RenderWindow& window)
{
	if (UIElement::mouseInside(window)) {
		setRelativeTextureRect(1);
		return true;
	}
	else {
		setRelativeTextureRect(0);
		return false;
	}
}



void UISprite::setPosition(const sf::Vector2f& pos)
{
	sprite.setPosition(pos);
}


void UISprite::setHead(const sf::Vector2f& pos, const float& scale, const sf::RenderWindow& window)
{
	sf::FloatRect relTexRect(0, 0, 0.2, 0.2);
	sf::IntRect textRect = utils::relativeToGlobalTextureRect(relTexRect, texture);
	sprite.setTextureRect(textRect);
	setPosition(pos);
	//utils::scaleToFullScreenRatio(sprite, window, scale);
	setRelativeScale(scale, window);
	// setScale();
}

void UISprite::setRelativeScale(const float& scale, const sf::RenderWindow& window)
{
	utils::scaleToFullScreenRatio(sprite, window, scale);
}

void UISprite::setIgnoreCamera(bool ignore)
{
	ignoreCamera = ignore;
}



/*void UISprite::draw(sf::RenderWindow& window) const
{
	//Sprite::draw(window, sf::RenderStates());
	window.draw(*this);
	//window.draw(sprite);
}*/
