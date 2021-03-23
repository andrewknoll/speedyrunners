#pragma once
#include "SFML/Graphics.hpp"
//#include <Rect.hpp>
#include "Animation.h"
#include "Globals.hpp"
#include "Spritesheet.h"

#define PERIOD sf::milliseconds(1000/30)
typedef std::shared_ptr<Animation> AnimationPtr;

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
	std::shared_ptr<Animation> currentAnimation;
	mutable sf::Sprite mySprite;
	sf::Time countdown = PERIOD;

	std::vector<AnimationPtr> animations = std::vector<AnimationPtr>(glb::NUMBER_OF_ANIMATIONS);
	
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void getSpritesVectorFromMap(std::map<std::string, AnimationPtr> map);

public:
	//Character(sf::Rect<float> _hitBox = sf::Rect<float>(20,20,14, 30));
	Character(Spritesheet);
	void setPosition(const sf::Vector2f pos);
	void setPosition(float x, float y);
	void tickAnimation(sf::Time dT);
	void update(const sf::Time& dT);
	//Character(const std::string& _path);

	void setHorizontalAcc(float acc);
	void setVerticalSpeed(float vel);

	void setState(State s);
	State getState() const;

	sf::Sprite getSprite();
	

	void setAnimation(AnimationIndex i);
};

