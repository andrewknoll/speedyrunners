
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "utils.hpp" // physics



Character::Character(sf::Rect<float> _hitBox) 
	: hitBox(_hitBox), 
	RectangleShape(sf::Vector2f(_hitBox.width, _hitBox.height)),
	state(State::InAir)
{
	setPosition(_hitBox.left, _hitBox.top); // Del rectangulo
	setFillColor(sf::Color::Red);
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
		}
		else {
			vel.y = 0;
			acc.y = 0;
		}
		updateGrounded(collision->normal);

		hitBox.left = pos.x; hitBox.top = pos.y;
		setFillColor(sf::Color::Blue);
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
		setFillColor(sf::Color::Red);
	}
	setPosition(hitBox.left, hitBox.top); // Del rectangulo
}

void Character::updateGrounded(const sf::Vector2f& normal) {
	isGrounded = (normal == sf::Vector2f(0, -1));
	if (isGrounded) hasDoubleJumped = false;
	//std::cout << "grounded? " << isGrounded << "\n";
}

/*
void Character::update(const sf::Time& dT, const TileMap& tiles)
{
	float dtSec = dT.asSeconds();

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
		}
		else {
			vel.y = 0;
			acc.y = 0;
		}
		
		hitBox.left = pos.x; hitBox.top = pos.y;
		setFillColor(sf::Color::Blue);
		//vel = sf::Vector2f(0, 0);
		//acc = sf::Vector2f(0, 0);
		isGrounded = true; // supongo
		//sf::sleep(sf::seconds(2));
	}
	else {
		// Update vel:
		vel = utils::clampAbs(vel + acc * dtSec, physics::MAX_FALL_SPEED);

		// Update acc:
		updateAcceleration();
		// JUST DEBUG:
		setFillColor(sf::Color::Red);
	}
	setPosition(hitBox.left, hitBox.top); // Del rectangulo

	
}*/

void Character::setPosition(float x, float y)
{
	hitBox.left = x;
	hitBox.top = y;
	sf::RectangleShape::setPosition(x, y);
}

void Character::processInputs()
{
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
	else {
		isRunning = false;
		
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

void Character::updateAcceleration() {
	if (state == State::InAir)
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
		}
	}
	
}


/*Character::Character(const std::string& _path)
	: path(_path)
{
	texture.loadFromFile(_path + "animation_variant00.png");
	setTexture(texture);
	setTextureRect(sf::IntRect(300, 200, 100,100));
}*/
/*
void Character::draw(sf::RenderTarget& target) const {
	temporaryRectangle.draw(target);
}*/

