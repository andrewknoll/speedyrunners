#include "Item.h"

Item::Item(glb::item idx) : itemIndex(idx)
{
}

void Item::doThingTo(std::shared_ptr<Character> c)
{
	// Not here
}

glb::item Item::getItemIndex() const
{
	return itemIndex;
}
