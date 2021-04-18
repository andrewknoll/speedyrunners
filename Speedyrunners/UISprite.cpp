#include <iostream>
#include "UISprite.h"

void UISprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
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

/*void UISprite::draw(sf::RenderWindow& window) const
{
	//Sprite::draw(window, sf::RenderStates());
	window.draw(*this);
	//window.draw(sprite);
}*/
