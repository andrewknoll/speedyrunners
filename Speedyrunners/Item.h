#pragma once
#include "SFML/Graphics.hpp"
#include "Globals.hpp"

class Level;
class Character;
class Item : public sf::Drawable
{
protected:
	sf::Sprite icon;
	glb::item itemIndex;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
public:
	Item(glb::item idx);

	virtual bool update(sf::Time elapsed, const Level& lvl) = 0; // Returns true if the item should die (rocket explodes)
	
	virtual void doThingTo(std::shared_ptr<Character> c); // item, do your thing

	glb::item getItemIndex() const;
};

