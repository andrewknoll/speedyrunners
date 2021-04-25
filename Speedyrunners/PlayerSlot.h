#pragma once
#include "Character.h"
class PlayerSlot
{
	using CharPtr = std::shared_ptr<Character>;
protected:
	CharPtr me;
	int type;

public:
	virtual void setCharacter(CharPtr you);
	virtual CharPtr getCharacter() const;
	virtual int getType() const;
};

