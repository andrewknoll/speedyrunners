
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "utils.hpp" // physics
#include "Globals.hpp" //animation names
#include "Spritesheet.h"

Character::Character(Spritesheet sp): 
	hitBox(glb::default_hitbox) {
	getSpritesVectorFromMap(sp.get_animations());
	setAnimation(StartAnim);
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
	setFriction();
	float dtSec = dT.asSeconds();

	// New vel:
	//vel = physics::updateVelocity(vel, acc);
	vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);

	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;

	fixPosition(hitBox);
	auto collision = tiles.collision(hitBox);
	if (collision) {
		// std::cout << "n: " << collision->normal.x << "," << collision->normal.y << "\tpoint: " << collision->point.x << "," << collision->point.y << " " << collision->distance << "\n";
		// New position:
		sf::Vector2f pos(hitBox.left, hitBox.top);
		pos = pos + (collision->normal * (collision->distance));
		if (collision->normal.x != 0) { // Make 0 the component of the collision
			vel.x = 0;
			acc.x = 0;
			isRunning = false;
			if (isGrounded) {
				setAnimation(StandAnim);
			}
		}
		else {
			vel.y = 0;
			acc.y = 0;
		}
		updateGrounded(collision->normal);

		hitBox.left = pos.x; hitBox.top = pos.y;
		//setFillColor(sf::Color::Blue);
		//vel = sf::Vector2f(0, 0);
		//acc = sf::Vector2f(0, 0);
		//sf::sleep(sf::seconds(2));
	}
	else {
		// Update vel:
		//vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);

		// Update acc:
		updateAcceleration();
		// JUST DEBUG:
		//setFillColor(sf::Color::Red);
	}
	setPosition(hitBox.left, hitBox.top); // Del rectangulo
	if (vel.y > 0) {
		if (hasDoubleJumped) {
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
	if (isRunning) setAnimation(RunAnim, true);
	//std::cout << "grounded? " << isGrounded << "\n";
}

void Character::run(bool right){
	std::cout << "Running " << right << " \n";
	isGrounded = true;
	//isRunning = true;
	facingRight = right;
	if (isGrounded){
		acc.x = runningAcceleration;
		if (!isRunning) {
			setAnimation(RunAnim, true);
		}
	}
	else
		acc.x = flyingAcceleration;
	if(!right){
		acc.x = -acc.x;
	}
	isRunning = true;
}

void Character::stop(){
	isRunning = false;
}

void Character::jump() {
	if (isGrounded) {
		vel.y = -jumpingSpeed;
		isGrounded = false;
		setAnimation(JumpAnim);
	}
	else if (!hasDoubleJumped) { // in air and hasnt double jumped yet
		vel.y = -jumpingSpeed;
		hasDoubleJumped = true;
		setAnimation(DoubleJumpAnim);
	}
	isGrounded = false;
}
/*
void Character::processInput(sf::Event& e)
{
	if (e.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			isRunning = true;
			if (isGrounded)
				acc.x = runningAcceleration;
			else
				acc.x = flyingAcceleration;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			isRunning = true;
			if (isGrounded)
				acc.x = -runningAcceleration;
			else
				acc.x = -flyingAcceleration;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
			std::cout << "hasDJ: " << hasDoubleJumped << "\n";
			if (isGrounded) {
				vel.y = -jumpingSpeed;
			}
			else if (!hasDoubleJumped) { // in air and hasnt double jumped yet
				vel.y = -jumpingSpeed;
				hasDoubleJumped = true;
			}

			isGrounded = false;
		}
	}
	if (e.type == sf::Event::KeyReleased) {
		if (e.key.code == sf::Keyboard::Right || e.key.code == sf::Keyboard::Left) {
			isRunning = false;
		}
	}
	
}
*/
void Character::updateAcceleration() {
	if (!isGrounded)
		acc.y = physics::GRAVITY;
}

void Character::setFriction() {
	if (!isRunning) {
		float eps = 10;
		float friction = (isGrounded) ? physics::FLOOR_FRICTION : physics::AIR_FRICTION;
		if (vel.x > eps) {// pos vel, negative friction
			//std::cout << "Pa la dcha: " << vel.x << "\n";
			acc.x = -friction;
		}
		else if (vel.x < -eps) {// opposite
			//std::cout << "Pa la izq: " << vel.x << "\n";
			acc.x = friction;
		}
		else { // close to 0
			//std::cout << "Cerca de 0: " << vel.x << "\n";
			acc.x = 0;
			vel.x = 0;
			if (isGrounded){
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


void Character::getSpritesVectorFromMap(std::map<std::string, AnimationPtr> map) {
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