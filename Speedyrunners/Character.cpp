
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "utils.hpp" // physics
#include "Globals.hpp" //animation names


/*Character::Character(sf::Rect<float> _hitBox) : hitBox(_hitBox), RectangleShape(sf::Vector2f(_hitBox.width, _hitBox.height))
{
	setPosition(_hitBox.left, _hitBox.top); // Del rectangulo
	setFillColor(sf::Color::Red);
}*/

void Character::update(const sf::Time& dT)
{
	float dtSec = dT.asSeconds();
	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;
	setPosition(hitBox.left, hitBox.top); // Del rectangulo
	// Update vel:
	vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);
	// Update acc:
	acc.y = physics::GRAVITY;

	switch (state) {
	case State::Standing:
		setAnimation(StandAnim);
		break;
	case State::Running:
		setAnimation(RunAnim);
		break;
	case State::Sliding:
		setAnimation(SlideAnim);
		break;
	case State::WallJumping:
		setAnimation(WallHangAnim);
		break;
	}
}

/*Character::Character(const std::string& _path)
	: path(_path)
{
	texture.loadFromFile(_path + "animation_variant00.png");
	setTexture(texture);
	setTextureRect(sf::IntRect(300, 200, 100,100));
}*/

void Character::draw(sf::RenderTarget& target, sf::Time dT) {
	countdown -= dT;
	if (countdown <= sf::Time::Zero) {
		currentAnimation.advance_frame(mySprite);
		countdown = PERIOD;
	}
	target.draw(*this);
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	// apply the transform
	states.transform *= getTransform();

	// apply the animation texture
	states.texture = mySprite.getTexture();

	// draw the vertex array
	target.draw(mySprite, states);
}

void Character::setHorizontalAcc(float acc) {
	this->acc.x = acc;
}

void Character::setVerticalSpeed(float vel) {
	this->vel.y = vel;
}

void Character::setState(State s) {
	state = s;
}

Character::State Character::getState() const {
	return state;
}

void Character::setAnimation(AnimationIndex i) {
	if (animIdx != i) {
		currentAnimation = animations[i];
		mySprite = currentAnimation.get_first_frame();
	}
}


void Character::getSpritesVectorFromMap(std::map<std::string, Animation> map) {
	for (auto const& pair : map) {
		if (pair.first == glb::anim::STAND_ANIM) {
			animations[StandAnim] = pair.second;
		}
		else if (pair.first == glb::anim::RUN_ANIM) {
			animations[RunAnim] = pair.second;
		}
		else if (pair.first == glb::anim::SLIDE_ANIM) {
			animations[SlideAnim] = pair.second;
		}
		else if (pair.first == glb::anim::SLIDING_ANIM) {
			animations[SlidingAnim] = pair.second;
		}
		else if (pair.first == glb::anim::FALL_ANIM) {
			animations[FallAnim] = pair.second;
		}
		else if (pair.first == glb::anim::JUMP_ANIM) {
			animations[JumpAnim] = pair.second;
		}
		else if (pair.first == glb::anim::LONG_FALL_ANIM) {
			animations[LongFallAnim] = pair.second;
		}
		else if (pair.first == glb::anim::LONG_JUMP_ANIM) {
			animations[LongJumpAnim] = pair.second;
		}
		else if (pair.first == glb::anim::D_JUMP_ANIM) {
			animations[DoubleJumpAnim] = pair.second;
		}
		else if (pair.first == glb::anim::D_JUMP_FALL_ANIM) {
			animations[DoubleJumpFallAnim] = pair.second;
		}
		else if (pair.first == glb::anim::HOOK_SHOT_ANIM) {
			animations[HookshotAnim] = pair.second;
		}
		else if (pair.first == glb::anim::SWING_ANIM) {
			animations[SwingAnim] = pair.second;
		}
		else if (pair.first == glb::anim::RUNNING_HOOK_ANIM) {
			animations[RunningHookAnim] = pair.second;
		}
		else if (pair.first == glb::anim::WALL_HANG_ANIM) {
			animations[WallHangAnim] = pair.second;
		}
		else if (pair.first == glb::anim::FLIP_ANIM) {
			animations[FlipAnim] = pair.second;
		}
		else if (pair.first == glb::anim::ROLL_ANIM) {
			animations[RollAnim] = pair.second;
		}
		else if (pair.first == glb::anim::TUMBLE_ANIM) {
			animations[TumbleAnim] = pair.second;
		}
		else if (pair.first == glb::anim::GRABBED_ANIM) {
			animations[GrabbedAnim] = pair.second;
		}
		else if (pair.first == glb::anim::SKID_ANIM) {
			animations[SkidAnim] = pair.second;
		}
		else if (pair.first == glb::anim::SPIKED_ANIM) {
			animations[SpikedAnim] = pair.second;
		}
		else if (pair.first == glb::anim::TAUNT_ANIM) {
			animations[TauntAnim] = pair.second;
		}
		else if (pair.first == glb::anim::START_ANIM) {
			animations[StartAnim] = pair.second;
		}
	}
}