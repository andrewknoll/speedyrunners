#include "Globals.h"

#define NEW_HITBOX(str,w,h) hitboxes.insert(std::pair<std::string, sf::IntRect>(str, sf::IntRect(0,0,w,h)))
//TODO: Medir las que están comentadas.
Globals::Globals() {
	//NEW_HITBOX("Double-Jump-Fall", 1, 1.75);
	NEW_HITBOX("Double-Jump", 1, 1.75);
	//NEW_HITBOX("Flip", 1, 1.75);
	//NEW_HITBOX("Grabbed", 1, 1.75);
	NEW_HITBOX("Hookshot", 1, 1.75);
	NEW_HITBOX("Stand", 1, 1.75);
	//NEW_HITBOX("Long-Fall", 1, 1.75);
	//NEW_HITBOX("Long-Jump", 1, 1.75);
	//NEW_HITBOX("Roll", 1, 1.75);
	NEW_HITBOX("Run", 1, 1.75);
	NEW_HITBOX("Running-Hook", 1, 1.75);
	//NEW_HITBOX("Skid", 1, 1.75);
	NEW_HITBOX("Slide", 1, 0.8);
	NEW_HITBOX("Sliding", 1, 0.8);
	//NEW_HITBOX("Straight-Fall", 1, 1.75);
	//NEW_HITBOX("Straight-Jump", 1, 1.75);
	NEW_HITBOX("Swing", 1, 1.75);
	NEW_HITBOX("Taunt", 1, 1.75);
	//NEW_HITBOX("Spiked", 1, 1.75);
	NEW_HITBOX("Tumble", 1, 1.75);
	NEW_HITBOX("Wall-Hang", 1, 1.75);
}