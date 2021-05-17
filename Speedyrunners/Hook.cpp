#include "Hook.h"
#include "Resources.h"
#include "Line.hpp"
#include "utils.hpp"

Hook::Hook() : audioPlayer(Resources::getInstance().getAudioPlayer())
{
	sprite.setTexture(Resources::getInstance().getMiscTexture(glb::HOOK_TEX));
	setTextureRect(false);
	//hitBox = sprite.getGlobalBounds();
}

void Hook::setTextureRect(bool hooked) {
	auto size = Resources::getInstance().getMiscTexture(glb::HOOK_TEX).getSize(); // whole texture size
	sf::IntRect rect(sf::Vector2i(0, 0), sf::Vector2i(size.x / 2, size.y)); // first horizontal half
	if (hooked) { // if its hooked, second half
		rect.left += rect.width;
	}
	sprite.setTextureRect(rect);
	utils::centerOrigin(sprite);
}

void Hook::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (use) {
		selbaward::Line line(hookerPosition, getPosition());
		line.setThickness(4);
		line.setColor(sf::Color::Black);
		target.draw(line, states);
		target.draw(sprite, states);
	}
}

//Return 1 if hit a grappable surface
//Return -1 if hit a non-grappable surface
//Return 0 if all went ok
int Hook::update(const sf::Time& dT, const TileMap& tiles, const sf::Vector2f& pos) {
	//hitBox.left = getPosition().x; // not centered
	//hitBox.top = getPosition().y;
	if (hooked) {
		offset.y = -sin(angle()) * glb::FEET_TO_HAND.y;
		offset.x = -cos(angle()) * glb::FEET_TO_HAND.x;
	}
	

	hookerPosition = pos + offset;
	if (!hooked && use) {
		relPosition = relPosition + vel * dT.asSeconds();
		setPosition(hookerPosition + relPosition);
		sprite.setPosition(hookerPosition + relPosition);
		sprite.setRotation(utils::degrees(atan2f(vel.y, vel.x)) + 90.0f);

		hitBox = sprite.getGlobalBounds(); // centered
		auto collisions = tiles.collision(hitBox);
		if (!collisions.empty()) {
			auto c = collisions.front();
			if (c.tileType == Tiles::GRAPPLABLE) {
				sprite.setRotation(0.0);
				vel = sf::Vector2f(0.0, 0.0);
				hooked = true;
				setTextureRect(hooked);
				audioPlayer.play(AudioPlayer::Effect::GRAPPLINGHOOK_ATTACH);
				return 1;
			}
			else {
				audioPlayer.play(AudioPlayer::Effect::GRAPPLINGHOOK_MISS); // hit a black tile
				return -1;
			}
		}
	}
	else return 1;
	return 0;
}

void Hook::fire(const sf::Vector2f& pos, bool facingRight)
{
	hooked = false;
	use = true;
	relPosition = sf::Vector2f(0.0, 0.0);
	setPosition(hookerPosition);
	vel = sf::Vector2f(1000, -1000);
	offset = glb::FEET_TO_HAND;
	if (!facingRight) {
		vel.x = -vel.x;
		offset.x = -offset.x;
	}
	audioPlayer.play(AudioPlayer::Effect::GRAPPLINGHOOK_SHOOT);
}

sf::Vector2f Hook::radius() const {
	return hookerPosition - getPosition();
}

float Hook::angle() const {
	return atan2(-radius().y, radius().x);
}

sf::Vector2f Hook::tangent() const {
	return sf::Vector2f(-sin(angle()), -cos(angle()));
}

void Hook::destroy() {
	hooked = false;
	use = false;
	relPosition = sf::Vector2f(0.0, 0.0);
	setPosition(hookerPosition + relPosition);
}
