
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Character.h"
#include "utils.hpp" // physics
#include "Globals.hpp" //animation names
#include "Spritesheet.h"
#include "Rocket.h"

Character::Character(Spritesheet sp) :
	hitBox(glb::default_hitbox){
	animations = sp.get_animations();
	setAnimation(StartAnim);
	this->setScale(0.45, 0.45);
	auto o = getOrigin();
	setOrigin(o.x-hitBox.width, o.y-hitBox.height*2.25);
	updateHitBoxRectangle();
}

sf::Sprite Character::getSprite() {
	return mySprite;
}

void Character::setPosition(float x, float y) {
	//mySprite.setPosition(x, y);
	hitBox.top = y;
	hitBox.left = x;
	sf::Transformable::setPosition(x, y);
}

void Character::setPosition(const sf::Vector2f pos) {
	setPosition(pos.x, pos.y);
}

void Character::fixPosition(sf::FloatRect& hitbox) {
	if (hitbox.top < 0) {
		hitbox.top = 0;
		acc.y = physics::GRAVITY;
		vel.y = 0;
	}
	if (hitbox.left < 0) {
		hitbox.left = 0;
		acc.x = 0;
		vel.x = 0;
	}
}

/**
enum NORMAL_DIR {
	UP,
	UP_R,
	R,
	DOWN_R,
	DOWN,
	DOWN_L,
	L,
	UP_L
};

NORMAL_DIR direction(const sf::Vector2f& n) {

}*/
void Character::updateInRamp(Tiles::Ramp ramp) {
	if (ramp != Tiles::Ramp::NONE) {
		if (!inRamp && isGrounded) {
			std::cout << "Just hit a ramp, old vel = \t\t" << vel;
			vel = base * vel;
			inRamp = true;
			std::cout << "\nnew vel = \t\t" << vel << "\n";
		}
	}
	else {
		if (inRamp && isGrounded) {
			std::cout << "Just left a ramp, old vel = \t\t" << vel;
			vel = base * vel;
			inRamp = false;
			std::cout << "\nnew vel = \t\t" << vel << "\n";
		}
	}
	
}

void Character::setBaseFromRamp(Tiles::Ramp ramp) {
	using namespace Tiles;
	if (ramp == Ramp::DOWN || ramp == Ramp::CEIL_DOWN) {
		base = geometry::Mat2(
			geometry::normalize(sf::Vector2f(1.0f, 1.0f)),
			geometry::normalize(sf::Vector2f(1.0f, 1.0f))
		);
		
	} 
	else if (ramp == Ramp::UP || ramp == Ramp::CEIL_UP) {
		base = geometry::Mat2(
			geometry::normalize(sf::Vector2f(1.0f, -1.0f)),
			geometry::normalize(sf::Vector2f(-1.0f, -1.0f))
		);
		
	}
	else { // Floor or ceil
		base = geometry::Mat2(
			geometry::normalize(sf::Vector2f(1.0f, 0)),
			geometry::normalize(sf::Vector2f(0, -1.0f))
		);
		
	}
	if (!facingRight) base.front = -base.front; // if going left, swap front vector
	// TODO: Fix this!!!!  updateInRamp(ramp);
	// Rotate the sprite based on the ramp (normal):
	setRotation(180-utils::degrees(atan2(base.up.x, base.up.y)));
	// Mala idea, se repite cada ciclo:
	

}

void Character::updateVel(const float& dtSec) {
	if (!swinging) {
		sf::Vector2f runningSpeed = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);
		//if signs of velocity and acceleration are opposed,
		//or the running speed of the character is greater than their current velocity, set it to that
		if (((vel.x >= 0) ^ (acc.x >= 0)) || abs(vel.x) <= abs(runningSpeed.x)) vel.x = runningSpeed.x;
		//Otherwise slow down
		else if (isGrounded) {
			if (facingRight) vel.x = vel.x - physics::FLOOR_FRICTION * 0.5 * dtSec;
			else vel.x = vel.x + physics::FLOOR_FRICTION * 0.5 * dtSec;
		}
		//// TODO: donde poner esto????????????? 
		if (isGrounded) {
			//vel = base * vel;
		}
		if (false);
		else { // if not grounded,
			//Likewise with y axis
			if ((vel.y >= 0) ^ (acc.y >= 0)) vel.y = vel.y + acc.y * dtSec;
			else if (abs(vel.y) < abs(runningSpeed.y)) vel.y = runningSpeed.y;// vel.y = (vel.y > 0) ? runningSpeed.y : -runningSpeed.y;
			//Except we do nothing otherwise
		}
		//if running on the ground, velocity and acceleration are oposed
		if (isGrounded && isRunning && !usingHook) {
			if (((vel.x >= 0) ^ (acc.x >= 0)) && abs(vel.x) > 50.0f) {
				setAnimation(SkidAnim, true);
			}
			else {
				setAnimation(RunAnim, true);
			}
		}
	}
	else {
		vel = utils::length(hook.radius()) * hook.tangent() * omega;
		if (facingRight) setAnimationAngle(-135.0f - utils::degrees(hook.angle()));
		else setAnimationAngle(-45.0f - utils::degrees(hook.angle()));
	}
}

void Character::update(const sf::Time& dT, const TileMap& tiles)
{
	updateRunning();
	setFriction();
	float dtSec = dT.asSeconds();
	sf::Vector2f runningSpeed = vel;


	// New vel:
	updateVel(dtSec);
	

	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;

	fixPosition(hitBox);
	std::vector<Tiles::Collision> collisions = tiles.collision(hitBox, isGrounded);

	if (!collisions.empty()) {
		Tiles::Collision c = collisions.front();
		//for (const auto& c : collisions) {
		// std::cout << "n: " << c.->normal.x << "," << c.->normal.y << "\tpoint: " << c.->point.x << "," << c.->point.y << " " << c.->distance << "\n";
		// New position:
		sf::Vector2f pos(hitBox.left, hitBox.top);
		pos = pos + (c.normal * (c.distance));

		if (c.normal.y >= 0) useHook(false);

		// may be ramp
		using namespace Tiles;
		auto ramp = Tiles::toRamp(c.tileType);
		setBaseFromRamp(ramp);
		if (ramp == Ramp::NONE) {
			if (c.normal.x != 0) { // Make 0 the component of the collision
				vel.x = 0;
				acc.x = 0;
				isRunning = false;
				sliding = false;
				if (isGrounded && !usingHook && !sliding) {
					setAnimation(StandAnim);
				}
				if (c.tileType == Tiles::JUMP_WALL_L) {
					if (!isAtWallJump) {
						acc.y = 0;
						vel.y = 0;
					}
					facingRight = false;
					isAtWallJump = true;
					setAnimation(WallHangAnim);
				}
				else if (c.tileType == Tiles::JUMP_WALL_R) {
					if (!isAtWallJump) {
						acc.y = 0;
						vel.y = 0;
					}
					facingRight = true;
					isAtWallJump = true;
					setAnimation(WallHangAnim);
				}
			}
			else if (!swinging) {
				vel.y = 0;
				acc.y = physics::GRAVITY;
			}
		}
		else { // Ramp

		}
		
		if (sliding) setAnimation(SlidingAnim);
		updateGrounded(c.normal);

		hitBox.left = pos.x; hitBox.top = pos.y;
	}
	else { // No collision
		isGrounded = false;
	}

	updateAcceleration();
	setPosition(hitBox.left, hitBox.top); // Del rectangulo
	if (usingHook) {
		int res = hook.update(dT, tiles, getPosition());
		if (res == -1) {
			swinging = false;
			usingHook = false;
			hook.destroy();
		}
		else if (res == 1 && !swinging) {
			if (vel.x < 500.0f) vel.x = 500.0f;
			setAnimation(SwingAnim);
			omega = utils::length(vel) / utils::length(hook.radius());
			if (!facingRight) omega = -omega;
			swinging = true;
		}
	}
	else if (vel.y > 0) {
		if (isAtWallJump && vel.x == 0) {
			setAnimation(WallHangAnim);
		}
		else if (hasDoubleJumped) {
			setAnimation(DoubleJumpFallAnim);
		}
		else if (!isGrounded) {
			setAnimation(FallAnim);
		}
	}
	updateHitBoxRectangle(); // Debug
}

void Character::updateGrounded(const sf::Vector2f& normal) {
	isGrounded = (normal.y < 0);
	if (isGrounded) hasDoubleJumped = false;
}


void Character::updateRunning() {
	//If it's running, not swinging
	if (isRunning && !swinging) {
		if (isGrounded) {
			acc.x = runningAcceleration;
			/*float accy = acc.y;
			acc = base * sf::Vector2f(runningAcceleration,0);
			acc.y += accy * 5.0f;*/
		}
		else
			acc.x = flyingAcceleration;
		if (!facingRight) {
			acc.x = -acc.x;
		}
	}
}

void Character::run(bool right){
	//std::cout << "Running " << right << " \n";
	facingRight = right;
	isRunning = true;
}

void Character::stop(){
	isRunning = false;
}

void Character::slide() {
	if (isGrounded && isRunning) {
		setAnimation(SlideAnim);
		sliding = true;
		isRunning = false;
	}
}

void Character::stopSliding() {
	sliding = false;
}

void Character::jump() {
	if (isGrounded) {
		vel.y = -jumpingSpeed;
		isGrounded = false;
		setAnimation(JumpAnim);
	}
	else if (isAtWallJump) {
		vel.y = -jumpingSpeed * 0.75; 
		if (facingRight) vel.x = jumpingSpeed * 1.5;
		else vel.x = -jumpingSpeed * 1.5;
		setAnimation(JumpAnim);
		isAtWallJump = false;
		hasDoubleJumped = false;
	}
	else if (!hasDoubleJumped) { // in air and hasnt double jumped yet
		vel.y = -jumpingSpeed;
		hasDoubleJumped = true;
		setAnimation(DoubleJumpAnim);
		isAtWallJump = false;
	}
	isGrounded = false;
}
bool Character::isSwinging() const
{
	return swinging;
}
void Character::useHook(bool use)
{
	if (use) {
		if (!usingHook) { // just used it
			usingHook = true;
			hook.fire(getPosition(), facingRight);
			if (isGrounded && isRunning) {
				setAnimation(RunningHookAnim);
			}
			else setAnimation(HookshotAnim);
			
		}
	}
	else {
		swinging = false;
		usingHook = false;
		hook.destroy();
		setAnimation(DoubleJumpAnim);
	}
}

Character::ItemPtr Character::useItem(std::shared_ptr<Character> target) {
	//TODO - Item available?
	ItemPtr rocket = std::make_shared<Rocket>(getPosition(), target, facingRight);
	return rocket;
}

void Character::setDToCheckpoint(float d)
{
	dToCheckpoint = d;
}
float Character::getDToCheckpoint() const
{
	return dToCheckpoint;
}
void Character::setName(std::string s)
{
	name = s;
}
std::string Character::getName() const
{
	return name;
}
void Character::setSpritesheetsPath(std::string path)
{
	spritesheetsPath = path;
}
std::string Character::getSpritesheetsPath() const
{
	return spritesheetsPath;
}
std::string Character::getUIIconPath() const
{
	return getSpritesheetsPath() + "heads.png";
}

void Character::updateAcceleration() {
	if (!swinging)
		acc.y = physics::GRAVITY;
}

void Character::setFriction() {
	if (!isRunning && !swinging) {
		float eps = 10;
		float friction;
		if (isGrounded) {
			if (sliding) friction = physics::FLOOR_FRICTION * 1.5;
			else friction = physics::FLOOR_FRICTION;
		}
		else friction = physics::AIR_FRICTION;
		if (vel.x > eps) {// pos vel, negative friction
			acc.x = -friction;
		}
		else if (vel.x < -eps) {// opposite
			acc.x = friction;
		}
		else { // close to 0
			acc.x = 0;
			vel.x = 0;
			if (isGrounded && !usingHook){
				setAnimation(StandAnim);
			}
		}
	}
}

void Character::tickAnimation(sf::Time dT) {
	countdown -= dT;
	if (countdown <= sf::Time::Zero) {
		currentAnimation->advance_frame(mySprite);
		countdown = PERIOD;
	}
}

void Character::updateHitBoxRectangle() {
#ifdef DEBUG_HITBOX
	hitBoxShape.setPosition(hitBox.left, hitBox.top);
	hitBoxShape.setSize(sf::Vector2f(hitBox.width, hitBox.height));
	hitBoxShape.setFillColor((isGrounded) ? sf::Color::Red : sf::Color::Green);
#endif
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (usingHook) target.draw(hook);
	// apply the transform
	states.transform *= getTransform();

	// apply the animation texture
	states.texture = mySprite.getTexture();

	target.draw(mySprite, states);



#ifdef DEBUG_HITBOX
	target.draw(hitBoxShape);
#endif
}

void Character::setHorizontalAcc(float acc) {
	this->acc.x = acc;
}

void Character::setVerticalSpeed(float vel) {
	this->vel.y = vel;
}

void Character::setAnimation(AnimationIndex i, bool loop, bool reverse) {
	if (animIdx != i) {
		currentAnimation = animations[i];
		mySprite = currentAnimation->get_first_frame();
		animIdx = i;
	}
	currentAnimation->set_loop(loop);
	currentAnimation->set_reverse(reverse);
	currentAnimation->update_orientation(facingRight);
}

void Character::setAnimationAngle(float angle) {
	//currentAnimation->update_angle(mySprite, angle);
}
