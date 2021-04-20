
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

	// New vel:
	//vel = physics::updateVelocity(vel, acc);
	vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);

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
		if (c.collision.normal.x != 0) { // Make 0 the component of the collision
			vel.x = 0;
			acc.x = 0;
			isRunning = false;
			if (isGrounded) {
				setAnimation(StandAnim);
			}
			if (c.tileType == Tiles::JUMP_WALL_L) {
				facingRight = false;
				isAtWallJump = true;
				setAnimation(WallHangAnim);
			}
			else if (c.tileType == Tiles::JUMP_WALL_R) {
				facingRight = true;
				isAtWallJump = true;
				setAnimation(WallHangAnim);
			}
		}
		else {
			vel.y = 0;
			acc.y = physics::GRAVITY;
		}
		updateGrounded(c.collision.normal);

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
		if (isAtWallJump) {
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
	if (isRunning) setAnimation(RunAnim, true);
	//std::cout << "grounded? " << isGrounded << "\n";
}


void Character::updateRunning() {
	if (isRunning) {
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
	if (!isRunning && isGrounded) {
		setAnimation(RunAnim, true);
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
	else if (isAtWallJump) {
		vel.y = -jumpingSpeed;
		if (facingRight) vel.x = jumpingSpeed / 1.5;
		else vel.x = -jumpingSpeed / 1.5;
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