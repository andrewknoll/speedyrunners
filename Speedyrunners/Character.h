#pragma once

#include "SFML/Graphics.hpp"
#include "TileMap.h"
//#include <Rect.hpp>
#include "Animation.h"
#include "Globals.hpp"
#include "Spritesheet.h"
#include "Hook.h"
#include "utils.hpp"
#include "AudioPlayer.h"
#include "ParticleSystem.h"


#include "Item.h"

//#define DEBUG_HITBOX

class TileMap;
class Level;

//class Tiles::Ramp;

class Character : public sf::Transformable, public sf::Drawable
{
	using AnimationPtr = std::shared_ptr<Animation>;
	using ItemPtr = std::shared_ptr<Item>;

	// Item:
	glb::item currentItem = glb::item::NONE;
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
#ifdef DEBUG_HITBOX
	sf::RectangleShape hitBoxShape;
#endif

	// Base:
	geometry::Mat2 base = geometry::Mat2(sf::Vector2f(1,0), sf::Vector2f(0,1));
	bool inRamp = false;

	const sf::Time PERIOD = sf::milliseconds(1000 / 30);

	int myID;
	int skin_variant = 0;

	// for physics:
	sf::Vector2f vel; // Velocity
	sf::Vector2f acc; // Acceleration
	float omega; //angular speed

	sf::Time stunnedRemaining;
	bool isStunned = false; // spikes, rockets, etc
	bool isGrounded = false;
	bool hasDoubleJumped = false;
	bool dead = false;
	bool holdingJump = false;

	// For animation:
	AnimationIndex animIdx;
	bool facingRight = true;
	bool isRunning = false;
	bool isAtWallJump = false;
	bool sliding = false;

	// Boost
	float boostPower = 1.5; // 10% faster
	bool usingBoost = false;
	sf::Time maxBoostTime = sf::seconds(2);
	sf::Time remainingBoostTime = maxBoostTime;


	std::shared_ptr<Animation> currentAnimation;
	sf::Sprite mySprite;
	sf::Time countdown = PERIOD;
	sf::Time jumpCoolDown = sf::seconds(0.1);
	sf::Time currJumpCD = jumpCoolDown;

	std::vector<AnimationPtr> animations = std::vector<AnimationPtr>(glb::NUMBER_OF_ANIMATIONS);

	// parameters, here so they can be different e.g for bots:
	// Horizontal accelerations:
	float runningAcceleration = glb::runningAcceleration; // acceleration in the floor
	float flyingAcceleration = glb::flyingAcceleration; // or in the air
	float jumpingSpeed = glb::jumpingSpeed; // or in the air

	// Hook
	bool usingHook = false;
	bool swinging = false;
	Hook hook = Hook();

	// For checkpoints, distance to active checkpoint:
	float dToCheckpoint;

	// Name:
	std::string name;

	// All spritesheets path:
	std::string spritesheetsPath = glb::CONTENT_PATH + "Characters/Speedrunner/";

	// Audio:
	AudioPlayer &audioPlayer;
	
	sf::Vector2f lastSafePosition = sf::Vector2f();

	//Score
	int score = 0;

	// Particle systems:
	std::vector<particles::ParticleSystem>& particleSystems;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateAcceleration();
	void setFriction();
	
	void updateGrounded(const sf::Vector2f& normal);
	void updateStunned(const sf::Time& dT);
	void updateRunning();

	void updateHitBoxRectangle();
	void fixPosition(sf::FloatRect& hitbox);
	void updateVel(const float& dtSec);
	void updateInRamp(Tiles::Ramp ramp);
	void setDefaultOrigin();
	void updateBoost(const sf::Time& dT, const Level& lvl);
	void emitBrakeParticles();
public:
	Character(Spritesheet sp, int ID, int variant = 0);

	int getID() const;
	int getVariant() const;

	void useBoost(bool useIt = true);
	float getRemainingBoost01() const; // returns the remaining boost between 0 and 1

	bool getGrounded() const;
	void setPosition(const sf::Vector2f pos);
	void setPosition(float x, float y);
	sf::Vector2f getLastSafePosition() const;

	sf::Sprite getSprite();
	void tickAnimation(sf::Time dT);
	void setAnimation(AnimationIndex i, bool loop = false, bool reverse = false);
	void setAnimationAngle(float angle);

	void setBaseFromRamp(Tiles::Ramp ramp);

	void update(const sf::Time& dT, const Level& lvl);

	void setHorizontalAcc(float acc);
	void setVerticalSpeed(float vel);

	//void setState(State s);
	//State getState() const;

	bool getIsStunned() const;

	void run(bool right);
	void stop();
	void respawn(sf::Vector2f position);
	void die();
	void slide();
	void stopSliding();
	void startJumping();
	void stopJumping();

	bool isDead() const;
	bool isSwinging() const;
	bool canWallJump() const;
	
	sf::Vector2f getVelocity() const;
	sf::Vector2f getAccel() const;

	void increaseScore(int d);
	int getScore() const;
	void setScore(int score);
	
	void useHook(bool use=true);
	ItemPtr useItem(std::shared_ptr<Character> target = nullptr);


	void setDToCheckpoint(float d);
	float getDToCheckpoint() const;

	void setName(std::string s);
	std::string getName() const;

	void setSpritesheetsPath(std::string path);
	std::string getSpritesheetsPath() const;

	// for UI:
	std::string getUIIconPath() const;


	// Item:
	glb::item getCurrentItem() const;
	void setItem(glb::item item);

	const sf::FloatRect& getHitBox() const;


	// TODO: implementar estado de stunned, con su countdown, etc
	void getHitByRocket();
};
