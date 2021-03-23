#pragma once
#include "Character.h"
class Player
{
	using CharState = Character::State;
	using CharPtr = std::shared_ptr<Character>;
private:
	CharPtr me;
public:
	void captureEvents(const sf::Event& event);
	void setCharacter(CharPtr you);
	CharPtr getCharacter() const;
};

