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

	sourceAnim->set_scale(0.5f, 0.5f);

	source.setScale(0.5f, 0.5f);
	sf::Vector2f o = sourceAnim->get_origin_point();
	o.y /= 2;
	source.setOrigin(o);
	source = sourceAnim->get_first_frame();
	ttl = beamParticles.getSettings().ttl;

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::ICE_BEAM_FULL);

}

void IceRay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	if (!sourceAnim->getFinished()) {
		target.draw(source, states);
	}
	else {
		target.draw(beamParticles);
	}
	
}

bool IceRay::update(sf::Time elapsed, const Level& lvl) {
	sf::Vector2f pos = user->getPosition();
	
	
	if (!sourceAnim->getFinished()) {
		sf::Vector2f o = sourceAnim->get_origin_point();
		o.y /= 2;
		source.setOrigin(o);
		source.setPosition(pos);
		tickAnimation(elapsed);
		return false;
	}
	else if(ttl > sf::Time::Zero){
		if (!emitted && sourceAnim->get_current_frame() > 10) {
			beamParticles.emitLinear(sf::Vector2f(0, 0), cam.viewRectangle().width);
			emitted = true;
		}
		beamParticles.setScale(1.0f, 0.3f);
		beamParticles.setPosition(pos);

		beamParticles.update(elapsed);
		ttl -= elapsed;
		
		return false;
	}
	return true;
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


