#include <cmath>
#include "IceRay.h"
#include "Resources.h"
#include "utils.hpp"
#include "Resources.h"
#include "Level.h"

IceRay::IceRay(CharPtr user, bool facingRight) :
	Item(glb::item::ICERAY),
	user(user)
{
	auto& anim = Resources::getInstance().getMiscSpriteSheet(0);
	auto& t = Resources::getInstance().getItemTexture(glb::item::ICERAY);

	beamAnim = Animation(1, 11, t);

	//setTexRect(true);

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::ICE_BEAM_FULL);

}

void IceRay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	target.draw(source, states);
	if (utils::inRange(beamAnim.get_current_frame(), 2, 10)) {
		for (auto& s : beam) {
			target.draw(s, states);
		}
	}
	
}

bool IceRay::update(sf::Time elapsed, const Level& lvl) {
	sf::Vector2f pos = user->getPosition();
	source.setPosition(pos);
	int i = beamFrame;
	
	while (pos.x < lvl.getCollidableTiles().getWidth()) {
		pos.x = pos.x + beamWidth;
		sf::Sprite s;
		s.setTexture(beamTex);
		s.setTextureRect(beamRects[++i % beamRects.size()]);
		s.setPosition(pos);
		beam.push_back(s);
	}
	return false;
}


void IceRay::doThingTo(std::shared_ptr<Character> c)
{
	if (utils::inRange(c->getPosition().y - user->getPosition().y, -16, 16)) {

	}
}


