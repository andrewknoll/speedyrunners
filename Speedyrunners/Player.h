#pragma once
#include "Character.h"
#include "Globals.hpp"
class Player
{
	using CharPtr = std::shared_ptr<Character>;
private:
	CharPtr me;

	bool holdingX = false;
	bool holdingDown = false;
public:
	void setCharacter(CharPtr you);
	CharPtr getCharacter() const;

	virtual bool captureEvents(const sf::Event& event);
};

