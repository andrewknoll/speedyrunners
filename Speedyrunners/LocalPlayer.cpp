#include "LocalPlayer.h"

LocalPlayer::LocalPlayer(const Settings & settings, int id) : Player(settings, id){
}

LocalPlayer::Event LocalPlayer::captureEvents(const sf::Event& event) {
	if (!me->isDead()) {
		if (sf::Keyboard::isKeyPressed(k_left)) {
			me->run(false);
		}
		else if (sf::Keyboard::isKeyPressed(k_right)) {
			me->run(true);
		}
		if (sf::Keyboard::isKeyPressed(k_down)) {
			me->slide();
		}
		if (sf::Keyboard::isKeyPressed(k_boost)) {
			me->useBoost();
		}
		else {
			me->useBoost(false);
		}
		if (event.type == sf::Event::KeyPressed) {
#ifdef WEIRD_CHEATS
			if (event.key.code == sf::Keyboard::Key::T) {
				me->tumbleWithBox();
			}
			if (event.key.code == sf::Keyboard::Key::I) {
				int current = me->getCurrentItem();
				me->setItem(glb::item((current + 1) % (glb::NUMBER_OF_ITEMS - glb::NUMBER_OF_UNOBTAINABLE_ITEMS)));
			}
#endif
			if (event.key.code == k_jump && !holdingUp) {
				holdingUp = true;
				me->startJumping();
			}

			/*else if (event.key.code == sf::Keyboard::Key::Down) {
				if (me->getState() == CharState::Standing ||
					me->getState() == CharState::InAir ||
					me->getState() == CharState::Running) {
					//Comprobar si se puede mientras est�s en la pared
					//me->setHorizontalAcc(SLIDING_FRICTION);
				}

			}*/
			else if (event.key.code == k_hook && !holdingX) {
				me->useHook();
				holdingX = true;
			}
			else if (event.key.code == k_item) {
				return USE_ITEM;
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == k_right || event.key.code == k_left) {
				me->stop();
			}
			else if (event.key.code == k_hook) {
				holdingX = false;
				me->useHook(false);
			}
			else if (event.key.code == k_down) {
				me->stopSliding();
			}
			else if (event.key.code == k_jump) {
				holdingUp = false;
				me->stopJumping();
			}
		}
	}
	return NONE;
}
