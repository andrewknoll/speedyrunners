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
	sourceAnim = *Resources::getInstance().getMiscSpriteSheet(0).get_animations()[0];
	auto& t = Resources::getInstance().getItemTexture(glb::item::ICERAY);

	sourceAnim.set_scale(0.5f, 0.5f);
	sourceAnim.update_orientation(facingRight);

	source.setScale(0.5f, 0.5f);
	sf::Vector2f o = sourceAnim.get_origin_point();
	o.y /= 2;
	source.setOrigin(o);
	source = sourceAnim.get_first_frame();
	ttl = beamParticles.getSettings().ttl;
	displacement = sf::Vector2f((facingRight) ? 50 : -36, 0);

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::ICE_BEAM_FULL);

}

void IceRay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(particles, states);
	if (!emitted) {
		target.draw(source, states);
	}
	else {
		target.draw(beamParticles);
	}
	
}

int IceRay::update(sf::Time elapsed, const Level& lvl) {
	sf::Vector2f pos = user->getPosition();
	
	
	if (!sourceAnim.getFinished()) {
		sf::Vector2f o = sourceAnim.get_origin_point();
		o.y /= 2;
		source.setOrigin(o);
		source.setPosition(pos);
		tickAnimation(elapsed);
		return 0;
	}
	else if(ttl > sf::Time::Zero){
		if (!emitted) {
			auto w = cam.viewRectangle().width;
			beamParticles.emitLinear(displacement, facingRight? w : -w);
			emitted = true;
		}
		beamParticles.setScale(1.0f, 0.3f);
		beamParticles.setPosition(pos);

		beamParticles.update(elapsed);
		ttl -= elapsed;
		
		return 2;
	}
	return 1;
}

void IceRay::tickAnimation(sf::Time dT) {
	countdown -= dT;
	if (countdown <= sf::Time::Zero) {
		sourceAnim.advance_frame(source);
		countdown = glb::ANIMATION_PERIOD;
	}
}


void IceRay::doThingTo(std::shared_ptr<Character> c)
{
	auto pos_c = c->getPosition();
	auto pos_u = user->getPosition();
	if (c != user && utils::inRange(pos_c.y - pos_u.y, -16, 16)
		&& (facingRight? pos_c.x > pos_u.x : pos_c.x < pos_u.x)) {
		c->getFrozen();
	}
}
