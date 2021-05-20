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
	facingRight(user->isFacingRight()),
	beamParticles(Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ICE_BEAM_PS))
{
	sourceAnim = Resources::getInstance().getMiscSpriteSheet(0).get_animations()[0];
	auto& t = Resources::getInstance().getItemTexture(glb::item::ICERAY);

	source = sourceAnim->get_first_frame();
	//setTexRect(true);

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::ICE_BEAM_FULL);

}

void IceRay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	target.draw(source, states);
	if (sourceAnim->get_current_frame() > 3) {
		target.draw(beamParticles);
	}
	
}

bool IceRay::update(sf::Time elapsed, const Level& lvl) {
	sf::Vector2f pos = user->getPosition();
	
	source.setPosition(pos);
	tickAnimation(elapsed);

	beamParticles.emitLinear(pos, cam.viewRectangle().width);
	//beamParticles.setAllParticlesPosition(pos);

	return sourceAnim->getFinished();
}

void IceRay::tickAnimation(sf::Time dT) {
	countdown -= dT;
	if (countdown <= sf::Time::Zero) {
		sourceAnim->advance_frame(source);
		countdown = glb::ANIMATION_PERIOD;
	}
}


void IceRay::doThingTo(std::shared_ptr<Character> c)
{
	/*if (utils::inRange(c->getPosition().y - user->getPosition().y, -16, 16)) {

	}*/
}


