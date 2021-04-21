#include "Player.h"
#define NORMAL_RUNNING_ACC 2
#define JUMP_VEL -3
#define SLIDING_FRICTION -1

void Player::setCharacter(CharPtr you){
	me = you;
}

Player::CharPtr Player::getCharacter() const {
	return me;
}

void Player::captureEvents(const sf::Event& event) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		me->run(false);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		me->run(true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
		me->useHook();
	}
	else {
		me->useHook(false);
	}
	if (event.type == sf::Event::KeyPressed) {
		
		if (event.key.code == sf::Keyboard::Key::Z) {
			//TODO: Comprobar WallJumping
			//TODO: Considerar long presses
			me->jump();
		} 
		if (event.key.code == sf::Keyboard::Key::C) {
			// Item
			std::cout << "C pressed, TODO: items\n";
		}
		/*else if (event.key.code == sf::Keyboard::Key::Down) {
			if (me->getState() == CharState::Standing ||
				me->getState() == CharState::InAir ||
				me->getState() == CharState::Running) {
				//Comprobar si se puede mientras estás en la pared
				//me->setHorizontalAcc(SLIDING_FRICTION);
			}

		}*/
	}
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
			me->stop();
		}
	}
}
