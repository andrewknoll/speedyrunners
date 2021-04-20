#include "Player.h"

void Player::setCharacter(CharPtr you){
	me = you;
}

Player::CharPtr Player::getCharacter() const {
	return me;
}

bool Player::captureEvents(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Key::Left) {
			me->run(false);
		}
		else if (event.key.code == sf::Keyboard::Key::Right) {
			me->run(true);
		}
		else if (event.key.code == sf::Keyboard::Key::Z) {
			//TODO: Comprobar WallJumping
			//TODO: Considerar long presses
			me->jump();
		}
		/*else if (event.key.code == sf::Keyboard::Key::Down) {
			if (me->getState() == CharState::Standing ||
				me->getState() == CharState::InAir ||
				me->getState() == CharState::Running) {
				//Comprobar si se puede mientras estás en la pared
				//me->setHorizontalAcc(SLIDING_FRICTION);
			}

		}*/
		else if (event.key.code == sf::Keyboard::Key::C) {
			return true;
		}
	}
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
			me->stop();
		}
	}
	return false;
}
