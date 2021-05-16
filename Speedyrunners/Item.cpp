#include "Item.h"
#include "Resources.h"


Item::Item(glb::item idx) : itemIndex(idx), audioPlayer(Resources::getInstance().getAudioPlayer())
{
}

void Item::doThingTo(std::shared_ptr<Character> c)
{
	// Not here
}
void Item::changeState() {

}

glb::item Item::getItemIndex() const
{
	return itemIndex;
}
