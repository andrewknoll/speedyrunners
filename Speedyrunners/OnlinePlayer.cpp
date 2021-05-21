#include "OnlinePlayer.h"

OnlinePlayer::OnlinePlayer(sf::TcpSocket& socket, const Settings & settings, int id) : Player(settings, id), socket(socket)  {
}

OnlinePlayer::Event OnlinePlayer::captureEvents(const sf::Event& event) {
	sf::Packet packet;
	sf::Socket::Status status = socket.receive(packet);
	if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
		return DISCONNECTED;
	}

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
