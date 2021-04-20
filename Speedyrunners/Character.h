#pragma once
#include "SFML/Graphics.hpp"
#include "TileMap.h"
//#include <Rect.hpp>
#include "Animation.h"
#include "Globals.hpp"
#include "Spritesheet.h"

#define PERIOD sf::milliseconds(1000/30)
typedef std::shared_ptr<Animation> AnimationPtr;

class Character : public sf::Transformable, public sf::Drawable
{
public:
	//enum class State {Standing, Running, Sliding, InAir, Grappling, WallJumping, Stunned, Tripped};
	enum AnimationIndex {
		StandAnim, RunAnim, SlideAnim, SlidingAnim, FallAnim, JumpAnim,
		LongFallAnim, LongJumpAnim, DoubleJumpAnim, DoubleJumpFallAnim, HookshotAnim,
		SwingAnim, RunningHookAnim, WallHangAnim, FlipAnim, RollAnim, TumbleAnim,
		GrabbedAnim, SkidAnim, SpikedAnim, TauntAnim, StartAnim
	};
protected:
	//State state = State::InAir;

	sf::Rect<float> hitBox;

	sf::Vector2f vel; // Velocity
	sf::Vector2f acc; // Acceleration

	bool isGrounded = false;
	bool hasDoubleJumped = false;

	AnimationIndex animIdx;
	bool facingRight = true;
	bool isRunning = false;
	bool isAtWallJump = false;

	std::shared_ptr<Animation> currentAnimation;
	mutable sf::Sprite mySprite;
	sf::Time countdown = PERIOD;

	std::vector<AnimationPtr> animations = std::vector<AnimationPtr>(glb::NUMBER_OF_ANIMATIONS);
	
	// parameters, here so they can be different e.g for bots:
	// Horizontal accelerations:
	float runningAcceleration = glb::runningAcceleration; // acceleration in the floor
	float flyingAcceleration = glb::flyingAcceleration; // or in the air
	float jumpingSpeed = glb::jumpingSpeed; // or in the air


	// For checkpoints, distance to active checkpoint:
	float dToCheckpoint;

	// Name:
	std::string name;

	// All spritesheets path:
	std::string spritesheetsPath = glb::CONTENT_PATH + "Characters/Speedrunner/"; 

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateAcceleration();
	void setFriction();

	void updateGrounded(const sf::Vector2f& normal);

	void updateRunning();


public:
	Character(Spritesheet);
	
	void setPosition(const sf::Vector2f pos);
	void setPosition(float x, float y);

	sf::Sprite getSprite();
	void tickAnimation(sf::Time dT);
	void setAnimation(AnimationIndex i, bool loop = false, bool reverse = false);

	void update(const sf::Time& dT, const TileMap& tiles);

	void setHorizontalAcc(float acc);
	void setVerticalSpeed(float vel);

	//void setState(State s);
	//State getState() const;


	void run(bool right);
	void stop();
	void jump();


	void setDToCheckpoint(float d);
	float getDToCheckpoint() const;

	void setName(std::string s);
	std::string getName() const;

	void setSpritesheetsPath(std::string path);
	std::string getSpritesheetsPath() const;

	// for UI:
	std::string getUIIconPath() const;

};

