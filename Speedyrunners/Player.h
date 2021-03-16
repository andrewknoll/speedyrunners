#pragma once
#include "Character.h"
class Player
{
	using CharState = Character::State;
private:
	Character* me;
public:
	void captureEvents(sf::RenderWindow& window);
	void setCharacter(Character* you);
};

