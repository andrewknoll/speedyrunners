#include "Player.h"
#define NORMAL_RUNNING_ACC 2
#define JUMP_VEL -3
#define SLIDING_FRICTION -1

void Player::setCharacter(Character* you){
	me = you;
}

void Player::captureEvents(sf::RenderWindow& window) {
	sf::Event event;
	if (window.pollEvent(event)) {
		if (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			if (me->getState() == CharState::Standing) {
				me->setState(CharState::Running);
			}
			if (me->getState() == CharState::Running || me->getState() == CharState::InAir) {
				//me->setHorizontalAcc(-NORMAL_RUNNING_ACC);
			}
		}
		else if (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			if (me->getState() == CharState::Standing) {
				me->setState(CharState::Running);
			}
			if (me->getState() == CharState::Running || me->getState() == CharState::InAir) {
				//me->setHorizontalAcc(NORMAL_RUNNING_ACC);
			}
		}
		else if (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
			//Comprobar WallJumping
			if (me->getState() == CharState::Standing) {
				me->setState(CharState::InAir);
			}
			if (me->getState() == CharState::Running) {
				//Considerar long presses
				//me->setVerticalSpeed(JUMP_VEL);
			}
			else if (me->getState() == CharState::InAir) {
				//Doble salto
				//me->setVerticalSpeed(JUMP_VEL);
			}
		}
		else if (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			if (me->getState() == CharState::Standing ||
				me->getState() == CharState::InAir ||
				me->getState() == CharState::Running) {
				//Comprobar si se puede mientras estás en la pared
				//me->setHorizontalAcc(SLIDING_FRICTION);
			}
			
		}
	}
}