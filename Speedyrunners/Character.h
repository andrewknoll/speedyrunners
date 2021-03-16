#pragma once
#include "SFML/Graphics.hpp"
//#include <Rect.hpp>
#include "Animation.h"
#include "Globals.hpp"

#define PERIOD sf::milliseconds(1000/30)


class Character : public sf::Transformable, public sf::Drawable
{
public:
	enum class State {Standing, Running, Sliding, InAir, Grappling, WallJumping, Stunned, Tripped};
	enum AnimationIndex {
		StandAnim, RunAnim, SlideAnim, SlidingAnim, FallAnim, JumpAnim,
		LongFallAnim, LongJumpAnim, DoubleJumpAnim, DoubleJumpFallAnim, HookshotAnim,
		SwingAnim, RunningHookAnim, WallHangAnim, FlipAnim, RollAnim, TumbleAnim,
		GrabbedAnim, SkidAnim, SpikedAnim, TauntAnim, StartAnim
	};
protected:
	State state = State::Standing;

	sf::Rect<float> hitBox;
	//sf::RectangleShape temporaryRectangle;
	sf::Vector2f vel; // Velocity
	sf::Vector2f acc; // Acceleration

	bool isGrounded = false;
	bool hasDoubleJumped = false;

	AnimationIndex animIdx;
	mutable Animation currentAnimation;
	mutable sf::Sprite mySprite;
	sf::Time countdown = PERIOD;

	std::vector<Animation> animations = std::vector<Animation>(glb::NUMBER_OF_ANIMATIONS);
	
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	

public:
	//Character(sf::Rect<float> _hitBox = sf::Rect<float>(20,20,14, 30));
	void draw(sf::RenderTarget& target, sf::Time dT);
	void update(const sf::Time& dT);
	//Character(const std::string& _path);

	void setHorizontalAcc(float acc);
	void setVerticalSpeed(float vel);

	void setState(State s);
	State getState() const;

	void getSpritesVectorFromMap(std::map<std::string, Animation> map);

	void setAnimation(AnimationIndex i);
};

