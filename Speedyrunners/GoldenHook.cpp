#include "GoldenHook.h"
#include "Resources.h"
#include "utils.hpp"
#include "Line.hpp"
#include "Level.h"

void GoldenHook::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	selbaward::Line line(this->from->getPosition() + offset, position);
	line.setThickness(4);
	line.setColor(sf::Color::Black);
	target.draw(line, states);
	target.draw(hook);
}


void GoldenHook::setTexRect(bool first) {
	hook.setTextureRect(first ? rects[0] : rects[1]);
}

GoldenHook::GoldenHook(CharPtr _from, CharPtr _target, float width) :
	Item(glb::item::GOLDEN_HOOK),
	from(_from),
	target(_target),
	particleSyst(Resources::getInstance().getParticleSystem(glb::particleSystemIdx::ROCKET_SMOKE))
{
	auto& t = Resources::getInstance().getMiscTexture(glb::HOOK_TEX);
	hook.setTexture(t);
	hook.setColor(sf::Color(255, 215, 0));
	position = _from->getPosition() + offset;
	rects.emplace_back(utils::relativeToGlobalTextureRect(sf::FloatRect(0, 0, 0.5, 1), t)); // Top half only
	rects.emplace_back(utils::relativeToGlobalTextureRect(sf::FloatRect(0.5, 0, 0.5, 1), t)); // Bottom half only

	setTexRect(true);
	utils::setWidth(hook, width);
	utils::centerOrigin(hook);

	// direction of the hook:
	auto direction = _target->getPosition() - _from->getPosition();
	direction = direction / utils::length(direction);
	//std::cout << "Dir " << direction;
	if (direction.y > maxVertical) direction.y = maxVertical*std::abs(direction.x);
	else if (direction.y < -maxVertical) direction.y = -maxVertical*std::abs(direction.x);
	direction = direction / utils::length(direction);

	//std::cout << " -> " << direction <<"\n";

	angle = atan2f(direction.y, direction.x);
	//std::cout << "angle: " << angle << "\n";
	//if (angle > maxVertical) angle = maxVertical;
	//else if (angle < maxVertical) angle = -maxVertical;

	//direction.x = sin(angle);
	//direction.y = cos(angle);
	//angle:
	hook.setRotation(90+utils::degrees(angle));

	// vel:
	vel = linearVel * direction;

	// Sound:
	audioPlayer.play(AudioPlayer::Effect::GOLDEN_HOOK_SHOOT);
}

int GoldenHook::update(sf::Time elapsed, const Level& lvl)
{
	if (!hooked && !missed) {
		position += vel * elapsed.asSeconds();
		const auto& hookHB = hook.getGlobalBounds();
		if (hookHB.intersects(target->getHitBox())) { // hooks
			hooked = true; // todo cosas
			audioPlayer.play(AudioPlayer::Effect::GOLDEN_HOOK_HIT);
			setTexRect(false); // second sprite
		}
		else if (!lvl.getCollidableTiles().collision(hookHB).empty()) {
			audioPlayer.play(AudioPlayer::Effect::GRAPPLINGHOOK_MISS);
			missed = true;
			return missed; // should be destroyed
		}
	}
	else if (hooked) {
		// drag characters
		if (remainingPull > sf::Time::Zero) {
			const auto& p1 = from->getPosition(), p2 = target->getPosition();
			if (vel.x > 0 && p1.x >= p2.x || vel.x < 0 && p1.x <= p2.x) 
				return true; // already close enough
			from->pullTowards(p2, pullSpeed);
			target->bePulledTowards(p1, pullSpeed);
			position = target->getPosition(); // for hook
			remainingPull -= elapsed;
		}
		else {
			return true; // should be destroyed
		}
	}
	hook.setPosition(position);
	return false;
}

void GoldenHook::doThingTo(std::shared_ptr<Character> c)
{
}
