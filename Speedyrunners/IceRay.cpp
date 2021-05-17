#include <cmath>
#include "IceRay.h"
#include "Resources.h"
#include "utils.hpp"
#include "Resources.h"
#include "Level.h"

IceRay::IceRay(const Camera& cam, CharPtr user) :
	Item(glb::item::ICERAY),
	user(user),
	cam(cam),
	facingRight(user->isFacingRight())
{
	sourceAnim = Resources::getInstance().getMiscSpriteSheet(0).get_animations()[0];
	auto& t = Resources::getInstance().getItemTexture(glb::item::ICERAY);

	
	beamAnim = Animation(1, 11, t);
	
	beamAnim.set_loop(true);
	sf::Sprite s = beamAnim.get_first_frame();
	if (!facingRight) {
		sourceAnim->flip(source);
		beamAnim.flip(s);
	}
	beamWidth = s.getGlobalBounds().width;
	beam.push_back(s);
	

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
	for (int i = 0; i < beam.size(); i++) {
		pos.x += beamWidth;
	}
	
	source.setPosition(pos);
	
	while ((facingRight && pos.x < cam.viewRectangle().width) || (!facingRight && pos.x > cam.viewRectangle().left)) {
		if (facingRight) {
			pos.x = pos.x + beamWidth;
		}
		else {
			pos.x = pos.x - beamWidth;
		}
		sf::Sprite s = beam.back();
		beamAnim.advance_frame(s);
		s.setPosition(pos);
		beam.push_back(s);
	}
	tickAnimation(elapsed);
	return false;
}

void IceRay::tickAnimation(sf::Time dT) {
	countdown -= dT;
	if (countdown <= sf::Time::Zero) {
		sourceAnim->advance_frame(source);
		for (auto& b : beam) {
			beamAnim.advance_frame(b);
		}
		countdown = glb::ANIMATION_PERIOD;
	}
}


void IceRay::doThingTo(std::shared_ptr<Character> c)
{
	/*if (utils::inRange(c->getPosition().y - user->getPosition().y, -16, 16)) {

	}*/
}


