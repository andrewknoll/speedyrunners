
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

int Character::getCheckpointCounter() const{
	return checkpointCounter;
}

int Character::getLastSafeCheckpoint() const {
	return lastSafeCheckpoint;
}

void Character::resetCheckpointInfo(int lsc, int cc) {
	lastSafeCheckpoint = lsc;
	checkpointCounter = cc;
}

void Character::setLastSafeCheckpoint(int lsc) {
	if (lastSafeCheckpoint != lsc) {
		std::cout << lsc << checkpointCounter << std::endl;
		if (lsc == (lastSafeCheckpoint + 1) % n_checkpoints) {
			checkpointCounter++;
		}
		else if (lsc == (lastSafeCheckpoint - 1) % n_checkpoints) {
			checkpointCounter--;
		}		
		lastSafeCheckpoint = lsc;
	}
}

void Character::setNumberCheckpoints(int n) {
	n_checkpoints = n;
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
		if (!isGrounded && (vel.y > glb::SUPERSPEED_THRESHOLD * physics::MAX_FALL_SPEED) && vel.x >= 0) { // going to the right
			hasSuperSpeed = true;
			superSpeedRemaining = maxSuperSpeed;
		}
		base = geometry::Mat2(
			geometry::normalize(sf::Vector2f(1.0f, 1.0f)),
			geometry::normalize(sf::Vector2f(1.0f, 1.0f))
		);
		
	} 
	else if (ramp == Ramp::UP || ramp == Ramp::CEIL_UP) {
		if (!isGrounded  && (vel.y > glb::SUPERSPEED_THRESHOLD * physics::MAX_FALL_SPEED) && vel.x <= 0) { // to the left
			hasSuperSpeed = true;
			superSpeedRemaining = maxSuperSpeed;
		}
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

float Character::getMaxS() const {
	float maxS = physics::MAX_FALL_SPEED;
	//if (usingHook) return maxS;
	if (usingBoost) maxS *= boostPower;
	if (hasSuperSpeed) maxS *= superSpeedPower;
	return maxS;
}


void Character::updateVel(const float& dtSec) {
	/*if (isStunned) {
		acc.x = 0;
	}*/
	float maxS = getMaxS();
	if (!swinging) {
		if (usingBoost) acc *= boostPower;
		if (hasSuperSpeed) acc *= superSpeedPower;
		sf::Vector2f runningSpeed = utils::clampAbs(vel + acc * dtSec, maxS);
		//if signs of velocity and acceleration are opposed,
		//or the running speed of the character is greater than their current velocity, set it to that
		if (((vel.x >= 0) ^ (acc.x >= 0)) || abs(vel.x) <= abs(runningSpeed.x)) vel.x = runningSpeed.x;
		//Otherwise slow down
		else if (isGrounded) { // BUG: aqui no entra nunca
			if (rng::defaultGen.rand01() > 0.9) particleSystems[glb::particleSystemIdx::BRAKE].emit(getPosition());
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
	else { // is swinging
		vel = utils::length(hook.radius()) * hook.tangent() * omega;
		//std::cout << "swinging vel " << vel << "\n";
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
	if (hasSuperSpeed) {
		if (superSpeedRemaining <= sf::Time::Zero) {
			hasSuperSpeed = false;
		}
		superSpeedRemaining -= dT;
	}
}

void Character::updateStunned(const sf::Time& dT) {
	if (isStunned || isFrozen) {
		usingHook = false;
		isRunning = false;
		sliding = false;
		stunnedRemaining -= dT;
		if (isStunned) {
			rotate(3000.0 * dT.asSeconds()); // TODO: BUG: No funciona y no se por que!
		}
		
		if (stunnedRemaining < sf::Time::Zero) {
			isStunned = false;
			isFrozen = false;
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
	bool right = isAtWallJump ? !facingRight : facingRight;
	if (!right) { // facing left, back is to the right
		x = hitBox.left + hitBox.width + distance;
	}
	else {
		x = hitBox.left - distance;
	}
	return sf::Vector2f(x, y);
}


void Character::updateHook(sf::Time dT, const TileMap& tiles) {
	// hook
	if (usingHook) {
		int res = hook.update(dT, tiles, getPosition());
		if (res == -1 || (facingRight && utils::degrees(hook.angle()) > 15) || (!facingRight && utils::degrees(hook.angle()) > 165)) {
			swinging = false;
			usingHook = false;
			hook.destroy();
		}
		else if (res == 1) { // is swinging
			if (hasSuperSpeed) {
				superSpeedRemaining = maxSuperSpeed; // time starts when we stop swinging
			}
			if (!swinging) { // starts swinging
				if (vel.y > glb::SUPERSPEED_THRESHOLD * physics::MAX_FALL_SPEED) {
					hasSuperSpeed = true;
					superSpeedRemaining = maxSuperSpeed; // time starts when we stop swinging
				}
				//float velx = std::abs(vel.x);
				//if (velx < 500.0f) vel.x = 500.0f;

				auto v = vel;
				if (std::abs(v.x) < 500.0f) v.x = 500.0f;
				setAnimation(SwingAnim);
				omega = utils::length(v) / utils::length(hook.radius());
				if (!facingRight) omega = -omega;
				swinging = true;
				hasDoubleJumped = false; // resets when swinging
			}
		}
	} // end of hook
	else {
		swinging = false;
	}
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
	/*if (usingBoost) { // updating dT is the same as updating all speeds and acc
		dtSec *= boostPower;
	}
	if (hasSuperSpeed) dtSec *= superSpeedPower;
	*/

	sf::Vector2f runningSpeed = vel;

	if (currJumpCD > sf::Time::Zero) {
		currJumpCD = currJumpCD - dT;
	}
	else {
		currJumpCD = sf::Time::Zero;
	}

	if (frozenWiggleCooldown > sf::Time::Zero) {
		frozenWiggleCooldown -= dT;
	}
	else {
		frozenWiggleCooldown = sf::Time::Zero;
	}

	if (currHookCD > sf::Time::Zero) {
		currHookCD = currHookCD - dT;
	}
	else {
		currHookCD = sf::Time::Zero;
	}
	updateHook(dT, tiles);

	// New vel:
	updateVel(dtSec);

	
	// Move:
	hitBox.left += vel.x * dtSec;
	hitBox.top += vel.y * dtSec;
	//std::cout << swinging << " vely: " << vel.y << "\n";

	//fixPosition(hitBox);

	bool wasAtWallJump = isAtWallJump;
	if (!isFrozen && !isStunned && isAtWallJump) { // get out of walljump mode
		isAtWallJump = false; // we assume it is not, and set it if it is
		const std::vector<Tiles::Collidable>& side = tiles.tilesToTheSide(hitBox, !facingRight);// facing right, wall jump should be to the left, and viceversa
		Tiles::Collidable searching = (!facingRight) ? Tiles::Collidable::JUMP_WALL_L : Tiles::Collidable::JUMP_WALL_R;
		//std::cout << side.size() << " to the " << (facingRight ? "left" : "right")<< "\n";
		for (const auto& t : side) if (!isGrounded && t == searching) {
			isAtWallJump = true;
			break;
		}
		if (!isAtWallJump) setAnimation(AnimationIndex::LongFallAnim);
	}

	if (isFrozen) {
		if (stunnedRemaining <= glb::FREEZE_TIME / 2.0f) {
			if (iceCubeAnim.get_current_frame() == 0) {
				iceCubeAnim.advance_frame(iceCube);
				iceCube.setScale(0.65f, 0.65f);
				iceCube.setOrigin(iceCubeAnim.get_origin_point());
				iceCube.setColor(sf::Color(255, 255, 255, 150));
			}
		}
		else {
			iceCube = iceCubeAnim.get_first_frame();
			iceCube.setScale(0.65f, 0.65f);
			iceCube.setOrigin(iceCubeAnim.get_origin_point());
			iceCube.setColor(sf::Color(255, 255, 255, 150));
		}
		if (hasFrozenWiggled) {
			//Move to a random relative position
			float wAmount = rng::defaultGen.rand(0, 6);
			if (wiggleFrames % 2 == 1) {
				wAmount = -wAmount;
			}
			hitBox.left += wAmount;
			setPosition(hitBox.left, hitBox.top);
			stunnedRemaining -= sf::seconds(0.1);
			if (wiggleFrames <= 0) { //See tickAnimation
				hasFrozenWiggled = false;
				wiggleFrames = glb::WIGGLE_FRAMES;
			}
			else {
				wiggleFrames--;
			}
		}
	}

	sf::Vector2f posIni(hitBox.left, hitBox.top);
	//std::cout << "After move: " << posIni << "\n";
	std::vector<Tiles::Collision> collisions = tiles.collision(hitBox, isGrounded);

	if (!collisions.empty()) {
		Tiles::Collision c = collisions.front();
		//for (const auto& c : collisions) {
		// std::cout << "n: " << c.->normal.x << "," << c.->normal.y << "\tpoint: " << c.->point.x << "," << c.->point.y << " " << c.->distance << "\n";
		// New position:
		auto pos = posIni + (c.normal * (c.distance));
		//std::cout << "newpos: " << pos << "\n";
		if (c.normal.y >= 0) {
			useHook(false);
			/*vel.y = 0;
			acc.y = physics::GRAVITY;*/
		}

		// may be ramp
		using namespace Tiles;
		auto ramp = Tiles::toRamp(c.tileType);
		setBaseFromRamp(ramp);

		if (ramp == Ramp::NONE) {
			if (c.normal.x != 0) { // Make 0 the component of the collision
				float xSpeed = 0.75*std::abs(vel.x); // for walljumps
				vel.x = 0;
				acc.x = 0;
				isRunning = false;
				sliding = false;
				if (!isStunned && !tumble && isGrounded && !usingHook && !sliding) {
					setAnimation(StandAnim);
				}
				if (c.tileType == Tiles::JUMP_WALL_L) {
					/*if (false&&vel.y>0 && !isAtWallJump) {
						acc.y = 0;
						vel.y = 0;
					}*/
					if (!isStunned && !isFrozen) {
						isAtWallJump = true;
						facingRight = false;
						setAnimation(WallHangAnim);
						vel.y = std::max(vel.y - xSpeed, -physics::MAX_FALL_SPEED);
					}
				}
				else if (c.tileType == Tiles::JUMP_WALL_R) {
					/*if (false&& vel.y>0 && !isAtWallJump) {
						acc.y = 0;
						vel.y = 0;
					}*/
					if (!isStunned && !isFrozen) {
						isAtWallJump = true;
						facingRight = true;
						setAnimation(WallHangAnim);
						vel.y = std::max(vel.y - xSpeed, -getMaxS()); // modified by boost and so on
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

	setPosition(hitBox.left, hitBox.top); // Del rectangulo
	updateAcceleration();
	
	if (usingHook);
	else if (!isStunned && !tumble) {
		if (isAtWallJump && vel.x == 0) {
			if (rng::defaultGen.rand01() > 0.8) particleSystems[glb::particleSystemIdx::BRAKE].emit(getPosition());
			setAnimation(WallHangAnim);
		}
		else if (vel.y > 0) {
			if (hasDoubleJumped) {
				setAnimation(DoubleJumpFallAnim);
			}
			else if (!isGrounded) {
				setAnimation(FallAnim);
			}
		}
		else if (!isGrounded && !isAtWallJump) {
			setAnimation(LongFallAnim);
		}
	}

	updateHitBoxRectangle(); // Debug
	if (isGrounded && !isAtWallJump) {
		lastSafePosition = getPosition();
	}
	addTrail((getPosition() - pIni) / dtSec); // recalculate vel for trail
}

void Character::addTrail(const sf::Vector2f& v) {
	if (hasSuperSpeed) { //utils::length(v) > 0.9*physics::MAX_FALL_SPEED ) {
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
		if (isGrounded && currJumpCD <= sf::Time::Zero) {
			if (!holdingJump) setAnimation(JumpAnim);
			isGrounded = false;
			holdingJump = true;
			vel.y = -jumpingSpeed * 0.9;
			isGrounded = false;
			currJumpCD = jumpCoolDown;
			audioPlayer.play(AudioPlayer::Effect::JUMP);
		}
		else if (isAtWallJump) {
			if (!holdingJump) setAnimation(JumpAnim);
			isGrounded = false;
			//vel.y = std::min(vel.y-jumpingSpeed, -jumpingSpeed); // if going up, keeps momentum
			vel.y = std::min(vel.y, -0.15f*jumpingSpeed);
			if (facingRight) vel.x = 1.4*jumpingSpeed;
			else vel.x = -1.4*jumpingSpeed;
			isAtWallJump = false;
			hasDoubleJumped = false;
			audioPlayer.play(AudioPlayer::Effect::JUMP);
		}
		else if (!hasDoubleJumped) { // in air and hasnt double jumped yet
			if (!holdingJump) setAnimation(DoubleJumpAnim);
			isGrounded = false;
			holdingJump = true;
			vel.y = -jumpingSpeed;
			hasDoubleJumped = true;
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
			if (!usingHook && currHookCD <= sf::Time::Zero) { // just used it
				currHookCD = 0.5f * jumpCoolDown;
				if (isGrounded && isRunning) {
					setAnimation(RunningHookAnim);
				}
				else setAnimation(HookshotAnim);
				usingHook = true;
				hook.fire(getPosition(), facingRight);
			}
		}
		else {
			if (usingHook) setAnimation(DoubleJumpAnim);
			swinging = false;
			usingHook = false;
			hook.destroy();
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

void Character::pullTowards(const sf::Vector2f& target, float pullSpeed)
{
	auto dir = target - getPosition();
	dir = dir / utils::length(dir); // normalize
	vel = dir * pullSpeed;
	if (!isGrounded) setAnimation(AnimationIndex::SwingAnim);
	else setAnimation(AnimationIndex::RunningHookAnim);
}


void Character::bePulledTowards(const sf::Vector2f& target, float pullSpeed)
{
	auto dir = target - getPosition();
	dir = dir / utils::length(dir); // normalize
	vel = dir * pullSpeed;
	setAnimation(AnimationIndex::GrabbedAnim);
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
	if (!isFrozen) {
		stunnedRemaining = glb::FREEZE_TIME;
		iceCube = iceCubeAnim.get_first_frame();
		iceCube.setScale(0.65f, 0.65f);
		iceCube.setOrigin(iceCubeAnim.get_origin_point());
		iceCube.setColor(sf::Color(255, 255, 255, 150));
		isFrozen = true;
	}
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
	if (usingHook) target.draw(hook);
	// apply the transform
	states.transform *= getTransform();

	// apply the animation texture
	states.texture = mySprite.getTexture();

	target.draw(mySprite, states);
	auto s = iceCube.getScale();

	states.transform.scale(iceCube.getScale());
	states.texture = iceCube.getTexture();
	if (isFrozen) target.draw(iceCube, states);



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
