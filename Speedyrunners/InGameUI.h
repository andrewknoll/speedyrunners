#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "UIElement.h"

class Character;

class InGameUI : public sf::Drawable
{
	using CharPtr = std::shared_ptr<Character>;
	std::vector<std::unique_ptr<UIElement>> elements;


	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


public:
	//InGameUI(std::vector<CharPtr> characters);


	void setCharacters(std::vector<CharPtr> characters);
};

