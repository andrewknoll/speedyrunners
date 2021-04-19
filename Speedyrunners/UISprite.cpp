#include <iostream>
#include "UISprite.h"
#include "utils.hpp"
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
