
#include <SFML/Graphics.hpp>
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
}

sf::Sprite Character::getSprite() {
	return mySprite;
}

void Character::setPosition(float x, float y) {
	//mySprite.setPosition(x, y);
	hitBox.top = y;
	hitBox.left = x;
	sf::Transformable::setPosition(x, y + 1.75 * glb::tileSize.y);
}

void Character::setPosition(const sf::Vector2f pos) {
	setPosition(pos.x, pos.y);
}

void fixPosition(sf::FloatRect& hitbox) {
	if (hitbox.top < 0) {
		hitbox.top = 0;
	}
	if (hitbox.left < 0) {
		hitbox.left = 0;
	}
}

void Character::update(const sf::Time& dT, const TileMap& tiles)
{
	updateRunning();
	setFriction();
	float dtSec = dT.asSeconds();
	sf::Vector2f runningSpeed = vel;


	// New vel:
	//vel = physics::updateVelocity(vel, acc);
	
	if (!swinging) {
		runningSpeed = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);

		//if signs of velocity and acceleration are opposed,
		//or the running speed of the character is greater than their current velocity, set it to that
		if (vel.x >= 0 ^ acc.x >= 0 || abs(vel.x) <= abs(runningSpeed.x)) vel.x = runningSpeed.x;
		//Otherwise slow down
		else if (isGrounded){
			if (facingRight) vel.x = vel.x - physics::FLOOR_FRICTION * 0.5 * dtSec;
			else vel.x = vel.x + physics::FLOOR_FRICTION * 0.5 * dtSec;
		}
		std::cout << vel.x << std::endl;

		//Likewise with y axis
		if (vel.y >= 0 ^ acc.y >= 0) vel.y = vel.y + acc.y * dtSec;
		else if (vel.y >= 0 ^ acc.y >= 0 || abs(vel.y) < abs(runningSpeed.y)) vel.y = runningSpeed.y;
		//Except we do nothing otherwise
	}
	else {
		vel = utils::length(hook.radius()) * hook.tangent() * omega;
		if (facingRight) setAnimationAngle(-135.0f - utils::degrees(hook.angle()) );
		else setAnimationAngle(-45.0f - utils::degrees(hook.angle()));
	}

	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;

	fixPosition(hitBox);
	auto collisions = tiles.collision(hitBox);

	if (!collisions.empty()) {
		auto c = collisions.front();
		//for (const auto& c : collisions) {
		// std::cout << "n: " << c.collision->normal.x << "," << c.collision->normal.y << "\tpoint: " << c.collision->point.x << "," << c.collision->point.y << " " << c.collision->distance << "\n";
		// New position:
		sf::Vector2f pos(hitBox.left, hitBox.top);
		pos = pos + (c.collision.normal * (c.collision.distance));

		if (c.collision.normal.y >= 0) useHook(false);

		if (c.collision.normal.x != 0) { // Make 0 the component of the collision
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
		if (sliding) setAnimation(SlidingAnim);
		updateGrounded(c.collision.normal);

		hitBox.left = pos.x; hitBox.top = pos.y;
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
		else {
			setAnimation(FallAnim);
		}
	}
}

void Character::updateGrounded(const sf::Vector2f& normal) {
	isGrounded = (normal == sf::Vector2f(0, -1));
	if (isGrounded) hasDoubleJumped = false;
	if (isRunning && !usingHook) setAnimation(RunAnim, true);
	//std::cout << "grounded? " << isGrounded << "\n";
}


void Character::updateRunning() {
	//If it's running, not swinging
	if (isRunning && !swinging) {
		if (isGrounded) {
			acc.x = runningAcceleration;
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
	if (!isRunning && isGrounded && !usingHook && !sliding) {
		setAnimation(RunAnim, true);
	}
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
		vel.y = -jumpingSpeed;
		if (facingRight) vel.x = jumpingSpeed;
		else vel.x = -jumpingSpeed;
		setAnimation(JumpAnim);
		isAtWallJump = false;
	}
	else if (!hasDoubleJumped) { // in air and hasnt double jumped yet
		vel.y = -jumpingSpeed;
		hasDoubleJumped = true;
		setAnimation(DoubleJumpAnim);
		isAtWallJump = false;
	}
	isGrounded = false;
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
	if (!isGrounded && !swinging)
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

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (usingHook) target.draw(hook);
	// apply the transform
	states.transform *= getTransform();

	// apply the animation texture
	states.texture = mySprite.getTexture();

	target.draw(mySprite, states);
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
	currentAnimation->update_angle(mySprite, angle);
}
