#pragma once
#include "Character.h"
class Player
{
	using CharPtr = std::shared_ptr<Character>;
private:
	CharPtr me;
public:
	void setCharacter(CharPtr you);
	CharPtr getCharacter() const;

	virtual void captureEvents(const sf::Event& event);
};

