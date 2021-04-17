#include <iostream>
#include "UISprite.h"

void UISprite::loadTexture(std::string texturePath)
{
	try {
		if (!texture.loadFromFile(texturePath)) {
			std::cerr << "Error loading texture " << texturePath << "\n";
		}
		else {
			std::cout << texturePath << " texture loaded\n";
			setTexture(texture);
		}
	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	
}

void UISprite::draw(sf::RenderWindow& window) const
{
	//Sprite::draw(window, sf::RenderStates());
	window.draw(*this);
	//window.draw(sprite);
}
