#include "PlayerSlot.h"

void PlayerSlot::setCharacter(CharPtr you) {
	me = you;
}

PlayerSlot::CharPtr PlayerSlot::getCharacter() const {
	return me;
}

int PlayerSlot::getType() const
{
	return type;
}