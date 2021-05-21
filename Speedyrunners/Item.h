#pragma once
#include "SFML/Graphics.hpp"
#include "Globals.hpp"

class Level;
class Character;
class AudioPlayer;

class Item : public sf::Drawable
{
protected:
	glb::item itemIndex;

	AudioPlayer& audioPlayer;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
public:
	Item(glb::item idx);

	// Returns 2 if the item should "do their thing" but not die
	// Returns 1 if the item should die (rocket explodes).
	//Returns 0 if nothing should happen
	virtual int update(sf::Time elapsed, const Level& lvl) = 0; 
	
	virtual void doThingTo(std::shared_ptr<Character> c); // item, do your thing

	virtual void changeState(); // activate an item (tnt)

	glb::item getItemIndex() const;
};

