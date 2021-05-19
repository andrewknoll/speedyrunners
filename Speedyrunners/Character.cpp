
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Character.h"
#include "utils.hpp" // physics
#include "Globals.hpp" //animation names
#include "Spritesheet.h"
#include "Resources.h"
#include "Level.h"
Character::Character(Spritesheet sp, int ID, int variant) :
	hitBox(glb::default_hitbox),
	myID(ID),
	audioPlayer(Resources::getInstance().getAudioPlayer()),
	skin_variant(variant),
	particleSystems(Resources::getInstance().getParticleSystems()),
	iceCubeAnim(2, 1, Resources::getInstance().getMiscTexture(glb::ICE_CUBE_TEX))
{
	animations = sp.get_animations();
	setAnimation(StartAnim);
	this->setScale(0.45, 0.45);
	setDefaultOrigin();
	updateHitBoxRectangle();
	setSpritesheetsPath(glb::CONTENT_PATH + "Characters/" + glb::characterNames[ID] + "/");
}

int Character::getID() const {
	return myID;
}

int Character::getVariant() const {
	return skin_variant;
}

void Character::setDefaultOrigin() {
	//auto o = getOrigin();
	setOrigin(-hitBox.width,-hitBox.height * 2.25);
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

sf::Vector2f Character::getLastSafePosition() const {
	return lastSafePosition;
}

bool Character::getGrounded() const {
	return isGrounded;
}

void Character::setPosition(const sf::Vector2f pos) {
	setPosition(pos.x, pos.y);
}

void Character::useBoost(bool useIt) {
	usingBoost = useIt && remainingBoostTime > sf::Time::Zero;
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

	// Rotate the sprite based on the ramp (normal):
	if (ramp == Tiles::Ramp::UP) {
		setDefaultOrigin();
		setRotation(-45);//setRotation(180-utils::degrees(atan2(base.up.x, base.up.y)));
	}
	else if (ramp == Tiles::Ramp::DOWN) {
		setOrigin(0, -hitBox.height * 1.1f);
		setRotation(45);
		//setDefaultOrigin();
		
	}
	else {
		setDefaultOrigin();
		setRotation(0);
	}
}

void Character::updateVel(const float& dtSec) {
	/*if (isStunned) {
		acc.x = 0;
	}*/
	if (!swinging) {
		sf::Vector2f runningSpeed = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);
		//if signs of velocity and acceleration are opposed,
		//or the running speed of the character is greater than their current velocity, set it to that
		if (((vel.x >= 0) ^ (acc.x >= 0)) || abs(vel.x) <= abs(runningSpeed.x)) vel.x = runningSpeed.x;
		//Otherwise slow down
		else if (isGrounded) { // BUG: aqui no entra nunca
			std::cout << "applying friction\n";
			if (rng::defaultGen.rand01() > 0.9) particleSystems[glb::particleSystemIdx::BRAKE].emit(getPosition());
			else std::cout << "ESTOY AQUI Y NOP\n";
			if (facingRight) vel.x = vel.x - physics::FLOOR_FRICTION * 0.5 * dtSec;
			else vel.x = vel.x + physics::FLOOR_FRICTION * 0.5 * dtSec;
		}

		//Likewise with y axis
		if ((vel.y >= 0) ^ (acc.y >= 0)) vel.y = vel.y + acc.y * dtSec;
		else if (abs(vel.y) < abs(runningSpeed.y)) vel.y = runningSpeed.y;// vel.y = (vel.y > 0) ? runningSpeed.y : -runningSpeed.y;
		//Except we do nothing otherwise
		
		//if running on the ground, velocity and acceleration are oposed
		if (!isStunned && !isFrozen && !tumble && isGrounded && isRunning && !usingHook) {
			if (((vel.x >= 0) ^ (acc.x >= 0)) && abs(vel.x) > 20.0f) {
				emitBrakeParticles();
				setAnimation(SkidAnim, true);
				currentAnimation->update_orientation(!facingRight);
			}
			else {
				setAnimation(RunAnim, true);
			}
		}
	}
	else {
		vel = utils::length(hook.radius()) * hook.tangent() * omega;
		if (!isStunned) {
			if (facingRight) setAnimationAngle(-135.0f - utils::degrees(hook.angle()));
			else setAnimationAngle(-45.0f - utils::degrees(hook.angle()));
		}
	}
	if (tumble) {
		tumblingTime -= sf::seconds(dtSec);
		if (tumblingTime < sf::Time::Zero) {
			tumble = false;
		}
		else {
			vel *= glb::tumbleSpeedReduction;
			setAnimation(TumbleAnim);
		}
	}
}

void Character::updateBoost(const sf::Time& dT, const Level& lvl) {
	if (usingBoost && utils::dot(vel, vel) > 0.01) {
		if (rng::defaultGen.rand01() > 0.7f) particleSystems[glb::particleSystemIdx::BOOST].emit(getPosition());
		remainingBoostTime -= dT;
		audioPlayer.loop(AudioPlayer::Effect::SPEEDBOOST);
		if (remainingBoostTime < sf::seconds(0)) {
			usingBoost = false;
			std::cout << "boost depleted\n";
			audioPlayer.stop(AudioPlayer::Effect::SPEEDBOOST);
		}
		audioPlayer.stop(AudioPlayer::Effect::CHARGE_GROUNDBOOST); // not charging
	}
	else {
		audioPlayer.stop(AudioPlayer::Effect::SPEEDBOOST); // Not using speedboost
		if (remainingBoostTime < maxBoostTime && lvl.insideBoostbox(getPosition())) { // Charging
			audioPlayer.loop(AudioPlayer::Effect::CHARGE_GROUNDBOOST);
			remainingBoostTime += dT;
		}
		else { // Stopped charging
			audioPlayer.stop(AudioPlayer::Effect::CHARGE_GROUNDBOOST);
		}
	}
}

void Character::updateStunned(const sf::Time& dT) {
	if (isStunned || isFrozen) {
		usingHook = false;
		isRunning = false;
		sliding = false;
		stunnedRemaining -= dT;
		rotate(3000.0 * dT.asSeconds()); // TODO: BUG: No funciona y no se por que!
		if (stunnedRemaining < sf::seconds(0)) {
			isStunned = false;
		}
	}
}


void Character::tumbleWithBox() {
	tumblingTime = glb::TUMBLE_TIME;
	tumble = true;
}

sf::Vector2f Character::getBackPosition(const float& distance) const
{
	float x, y = getPosition().y;
	if (!facingRight) { // facing left, back is to the right
		x = hitBox.left + hitBox.width + distance;
	}
	else {
		x = hitBox.left - distance;
	}
	return sf::Vector2f(x, y);
}

void Character::update(const sf::Time& dT, const Level& lvl)

{
	auto pIni = getPosition();
	updateStunned(dT);
	auto &tiles = lvl.getCollidableTiles();
	updateRunning();
	setFriction();
	float dtSec = dT.asSeconds();
	updateBoost(dT, lvl);
	if (usingBoost) { // updating dT is the same as updating all speeds and acc
		dtSec *= boostPower;
	}


	sf::Vector2f runningSpeed = vel;

	if (currJumpCD > sf::Time::Zero) {
		currJumpCD = currJumpCD - dT;
	}
	else {
		currJumpCD = sf::Time::Zero;
	}

	// New vel:
	updateVel(dtSec);
	
	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;

	fixPosition(hitBox);

	sf::Vector2f posIni(hitBox.left, hitBox.top);
	std::vector<Tiles::Collision> collisions = tiles.collision(hitBox, isGrounded);

	bool wasAtWallJump = isAtWallJump;
	if (!isFrozen && !isStunned && isAtWallJump) { // get out of walljump mode
		isAtWallJump = false; // we assume it is not, and set it if it is
		const std::vector<Tiles::Collidable>& side = tiles.tilesToTheSide(hitBox, !facingRight);// facing right, wall jump should be to the left, and viceversa
		Tiles::Collidable searching = (!facingRight) ? Tiles::Collidable::JUMP_WALL_L : Tiles::Collidable::JUMP_WALL_R;
		//std::cout << side.size() << " to the " << (facingRight ? "left" : "right")<< "\n";
		for (const auto& t : side) if (t == searching) {
			isAtWallJump = true;
			break;
		}
	}

	if (isFrozen) {
		if (stunnedRemaining <= glb::FREEZE_TIME / 2.0f) {
			iceCubeAnim.advance_frame(iceCube);
		}
		else {
			iceCube = iceCubeAnim.get_first_frame();
		}
		if (hasFrozenWiggled) {
			//Move to a random relative position
			move(sf::Vector2f(rng::defaultGen.rand(-2, 2), rng::defaultGen.rand(-2, 2)));
			stunnedRemaining -= sf::seconds(0.5);
			if (wiggleFrames <= 0) { //See tickAnimation
				hasFrozenWiggled = false;
				wiggleFrames = glb::WIGGLE_FRAMES;
			}
		}
	}

	if (!collisions.empty()) {
		Tiles::Collision c = collisions.front();
		//for (const auto& c : collisions) {
		// std::cout << "n: " << c.->normal.x << "," << c.->normal.y << "\tpoint: " << c.->point.x << "," << c.->point.y << " " << c.->distance << "\n";
		// New position:
		auto pos = posIni + (c.normal * (c.distance));

		if (c.normal.y >= 0) useHook(false);

		// may be ramp
		using namespace Tiles;
		auto ramp = Tiles::toRamp(c.tileType);
		setBaseFromRamp(ramp);

		if (ramp == Ramp::NONE) {
			if (c.normal.x != 0) { // Make 0 the component of the collision
				float xSpeed = 0.65*std::abs(vel.x); // for walljumps
				vel.x = 0;
				acc.x = 0;
				isRunning = false;
				sliding = false;
				if (!isStunned && !tumble && isGrounded && !usingHook && !sliding) {
					setAnimation(StandAnim);
				}
				if (c.tileType == Tiles::JUMP_WALL_L) {
					if (false&&vel.y>0 && !isAtWallJump) {
						acc.y = 0;
						vel.y = 0;
					}
					if (!isStunned && !isFrozen) {
						isAtWallJump = true;
						facingRight = false;
						setAnimation(WallHangAnim);
						vel.y = vel.y - xSpeed;
					}
				}
				else if (c.tileType == Tiles::JUMP_WALL_R) {
					if (false&& vel.y>0 && !isAtWallJump) {
						acc.y = 0;
						vel.y = 0;
					}
					if (!isStunned && !isFrozen) {
						isAtWallJump = true;
						facingRight = true;
						vel.y = vel.y - xSpeed;
						setAnimation(WallHangAnim);
					}
				}
			}
			else if (!swinging) {
				vel.y = 0;
				acc.y = physics::GRAVITY;
			}
		}
		
		if (sliding) {
			setAnimation(SlidingAnim);
			audioPlayer.continuePlaying(AudioPlayer::Effect::SLIDE);
		}
		updateGrounded(c.normal);

		hitBox.left = pos.x; hitBox.top = pos.y;
	}
	else { // No collision
		setBaseFromRamp(Tiles::Ramp::NONE);
		isGrounded = false;
	}

	updateAcceleration();
	setPosition(hitBox.left, hitBox.top); // Del rectangulo
	if (usingHook) {
		int res = hook.update(dT, tiles, getPosition());
		if (res == -1 || (facingRight && utils::degrees(hook.angle()) > 30) || (!facingRight && utils::degrees(hook.angle()) > 150)) {
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
			hasDoubleJumped = false; // resets when swinging
		}
	}
	else if (!isStunned && !tumble && vel.y > 0) {
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
	if (isGrounded && !isAtWallJump) {
		lastSafePosition = getPosition();
	}
	addTrail((getPosition() - pIni) / dtSec); // recalculate vel for trail
}

void Character::addTrail(const sf::Vector2f& v) {
	if (utils::length(v) > 0.9*physics::MAX_FALL_SPEED ) {
		//if (rng::defaultGen.rand01() > 0.0)
		particleSystems[glb::particleSystemIdx::SUPER_SPEED].emit(getPosition() - v * 0.01f, v);
	}
}

void Character::frozenWiggle() {
	if (frozenWiggleCooldown <= sf::Time::Zero) {
		hasFrozenWiggled = true;
		frozenWiggleCooldown = glb::FROZEN_WIGGLE_CD;
	}
}


void Character::updateGrounded(const sf::Vector2f& normal) {
	bool wasGrounded = isGrounded;
	isGrounded = (normal.y < 0);
	if (isGrounded) {
		hasDoubleJumped = false;
		if (!wasGrounded) 
			audioPlayer.play(AudioPlayer::Effect::FOLEY_LAND);
	}

}


void Character::updateRunning() {
	//If it's running, not swinging
	if (!isStunned && !isFrozen && isRunning && !swinging) {
		if (isGrounded) {
			acc.x = runningAcceleration;
			// audioPlayer.loop(AudioPlayer::Effect::FOOTSTEP); // Suena demasiado y no se pq
		}
		else
			acc.x = flyingAcceleration;
		if (!facingRight) {
			acc.x = -acc.x;
		}
	}
	else { // stopped running
		audioPlayer.stop(AudioPlayer::Effect::FOOTSTEP);
	}
}

void Character::run(bool right){
	if (isFrozen) {
		frozenWiggle();
	}
	else {
		if (!usingHook && !isAtWallJump) facingRight = right;
		if (!isAtWallJump) isRunning = true;
	}
}

void Character::stop(){
	isRunning = false;
}

void Character::respawn(sf::Vector2f position) {
	//TO-DO: Spawnear separados
	vel = sf::Vector2f(0, 0);
	acc = vel;
	dead = false;
	setPosition(position);
	setAnimation(StandAnim);
}

void Character::die() {
	stop();
	stopJumping();
	stopSliding();
	vel = sf::Vector2f(0, 0);
	dead = true;
}

void Character::slide() {
	if (!isStunned && !isFrozen && isGrounded && isRunning) {
		setAnimation(SlideAnim);
		sliding = true;
		isRunning = false;
	}
}

void Character::stopSliding() {
	sliding = false;
}

void Character::startJumping() {
	if (isFrozen) {
		frozenWiggle();
	}
	else if (!isStunned) {
		if (isGrounded && currJumpCD == sf::Time::Zero) {
			isGrounded = false;
			holdingJump = true;
			vel.y = -jumpingSpeed * 0.9;
			isGrounded = false;
			setAnimation(JumpAnim);
			currJumpCD = jumpCoolDown;
			audioPlayer.play(AudioPlayer::Effect::JUMP);
		}
		else if (isAtWallJump) {
			isGrounded = false;
			//vel.y = std::min(vel.y-jumpingSpeed, -jumpingSpeed); // if going up, keeps momentum
			vel.y = std::min(vel.y, 0.0f);// -jumpingSpeed * 0.3; // TODO: vel.x en funcion de vel.y?
			if (facingRight) vel.x = jumpingSpeed;
			else vel.x = -jumpingSpeed;
			setAnimation(JumpAnim);
			isAtWallJump = false;
			hasDoubleJumped = false;
			audioPlayer.play(AudioPlayer::Effect::JUMP);
		}
		else if (!hasDoubleJumped) { // in air and hasnt double jumped yet
			isGrounded = false;
			holdingJump = true;
			vel.y = -jumpingSpeed;
			hasDoubleJumped = true;
			setAnimation(DoubleJumpAnim);
			isAtWallJump = false;
			audioPlayer.play(AudioPlayer::Effect::DOUBLE_JUMP);
		}
	}
}

void Character::stopJumping() {
	holdingJump = false;
}

bool Character::isDead() const {
	return dead;
}

bool Character::isSwinging() const
{
	return swinging;
}

bool Character::canWallJump() const {
	return isAtWallJump;
}

sf::Vector2f Character::getVelocity() const {
	return vel;
}

sf::Vector2f Character::getAccel() const {
	return acc;
}

void Character::increaseScore(int d) {
	score += d;
}

bool Character::getIsStunned() const
{
	return isStunned;
}

int Character::getScore() const {
	return score;
}
void Character::setScore(int score) {
	this->score = score;
}
void Character::useHook(bool use)
{
	if (!isStunned) {
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
}

bool Character::canJump() const{
	return isGrounded || !hasDoubleJumped;
}

bool Character::isUsingHook() const {
	return usingHook;
}

bool Character::isUsingSlide() const {
	return sliding;
}

void Character::resetItem() {
	currentItem = glb::item::NONE; // Used item
}


void Character::setItemPtr(std::shared_ptr<Item> tnt) {
	itemPtr = tnt;
}

std::shared_ptr<Item> Character::getItemPtr() {
	return itemPtr;
}


void Character::getHitByRocket() {
	std::cout << "I got hit by a rocket :(\n"; // doesnt care that much yet
	setAnimation(SpikedAnim);
	stunnedRemaining = glb::STUN_TIME;
	isStunned = true;
}

void Character::getFrozen() {
	stunnedRemaining = glb::FREEZE_TIME;
	iceCube = iceCubeAnim.get_first_frame();
	isFrozen = true;
}


void Character::getHitByTNT(const sf::Vector2f& direction) {
	getHitByRocket();
	vel += direction;
}


void Character::setItem(glb::item item)
{
	currentItem = item;
}

const sf::FloatRect& Character::getHitBox() const
{
	return hitBox;
}


glb::item Character::getCurrentItem() const {
	return currentItem;
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
float Character::getRemainingBoost01() const
{
	return remainingBoostTime / maxBoostTime;
}
std::string Character::getUIIconPath() const
{
	return getSpritesheetsPath() + "heads.png";
}

void Character::updateAcceleration() {
	if (!swinging) {
		acc.y = physics::GRAVITY;
	}
	if (holdingJump) {
		acc.y = physics::GRAVITY * 0.7;
	}
		
}

void Character::emitBrakeParticles() {
	audioPlayer.continuePlaying(AudioPlayer::Effect::SLIDE_DOWN_SKID);
	if (rng::defaultGen.rand01() > 0.8)
		particleSystems[glb::particleSystemIdx::BRAKE].emit(getPosition()+sf::Vector2f(0,getSprite().getGlobalBounds().height/2.0f)); // sliding particles
}


void Character::setFriction() {
	if (!isRunning && !swinging) {
		float eps = 10;
		float friction;
		if (isGrounded) {
			if (std::abs(vel.x) > eps) {
				emitBrakeParticles();
				setAnimation(SkidAnim, true);
				currentAnimation->update_orientation(!facingRight);
			}
			if (sliding) friction = physics::FLOOR_FRICTION * 0.5;
			else friction = physics::FLOOR_FRICTION;
		}
		else friction = physics::AIR_FRICTION;
		if (isStunned) friction *= 2;
		if (isFrozen) friction /= 3;
		if (vel.x > eps) {// pos vel, negative friction
			acc.x = -friction;
		}
		else if (vel.x < -eps) {// opposite
			acc.x = friction;
		}
		else { // close to 0
			acc.x = 0;
			vel.x = 0;
			if (!isStunned && !tumble && isGrounded && !usingHook){
				setAnimation(StandAnim);
			}
		}
	}
}

void Character::tickAnimation(sf::Time dT) {
	if (!isFrozen) {
		countdown -= dT;
		if (countdown <= sf::Time::Zero) {
			currentAnimation->advance_frame(mySprite);
			countdown = glb::ANIMATION_PERIOD;
		}
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
	if (isFrozen) target.draw(iceCube);
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
	if (!isFrozen) {
		if (animIdx != i) {
			currentAnimation = animations[i];
			mySprite = currentAnimation->get_first_frame();
			animIdx = i;
		}
		currentAnimation->set_loop(loop);
		currentAnimation->set_reverse(reverse);
		currentAnimation->update_orientation(facingRight);
	}
}

void Character::setAnimationAngle(float angle) {
	//currentAnimation->update_angle(mySprite, angle);
}

bool Character::isFacingRight() const {
	return facingRight;
}
