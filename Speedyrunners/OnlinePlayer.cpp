#include "OnlinePlayer.h"
#include "OnlineRequest.h"

OnlinePlayer::OnlinePlayer(sf::TcpSocket& socket, const Settings & settings, int id) : Player(settings, id), socket(socket)  {
}

OnlinePlayer::Event OnlinePlayer::captureEvents(const sf::Event& event) {
	sf::Packet packet;
	sf::Socket::Status status = socket.receive(packet);
	if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
		return DISCONNECTED;
	}
	OnlineRequest req;
	packet >> req;

	if (!me->isDead()) {
		
		if (req.inputEvent.leftPressed) {
			me->run(false);
		}
		else if (req.inputEvent.rightPressed) {
			me->run(true);
		}
		if (req.inputEvent.downPressed) {
			me->slide();
		}
		if (req.inputEvent.boostPressed) {
			me->useBoost();
		}
		else {
			me->useBoost(false);
		}

		if (req.inputEvent.pressed) {
			if (req.inputEvent.inputEventKey == OnlineRequest::JUMP && !holdingUp) {
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
			else if (req.inputEvent.inputEventKey == OnlineRequest::HOOK && !holdingX) {
				me->useHook();
				holdingX = true;
			}
			else if (req.inputEvent.inputEventKey == OnlineRequest::ITEM) {
				return USE_ITEM;
			}
		}
		else {
			if (!req.inputEvent.leftPressed || !req.inputEvent.rightPressed) {
				me->stop();
			}
			if (req.inputEvent.inputEventKey == OnlineRequest::HOOK) {
				holdingX = false;
				me->useHook(false);
			}
			if (!req.inputEvent.downPressed) {
				me->stopSliding();
			}
			if (req.inputEvent.inputEventKey == OnlineRequest::JUMP) {
				holdingUp = false;
				me->stopJumping();
			}
		}
	}
	return NONE;
}
