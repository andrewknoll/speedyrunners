#include "OnlineRequest.h"

sf::Packet & operator<<(sf::Packet & packet, const OnlineRequest & o)
{
	if (o.type == OnlineRequest::KEY_EVENT) {
		return packet << (int)o.type << o.inputEvent.inputEventKey << o.inputEvent.pressed << o.inputEvent.leftPressed << o.inputEvent.rightPressed << o.inputEvent.downPressed << o.inputEvent.boostPressed;;
	}
	else if (o.type == OnlineRequest::CREATE || o.type == OnlineRequest::JOIN || o.type == OnlineRequest::RANDOM) {
		return packet << (int)o.type << o.stringParam;
	}
	else if (o.type == OnlineRequest::READY) {
		return packet << (int)o.type << o.stringParam << o.intParam << o.boolParam;
	}
	else if (o.type == OnlineRequest::CHARACTER_CHANGE) {
		return packet << (int)o.type << o.stringParam << o.intParam << (int)o.ciParam;
	}
	else {
		return packet << (int)o.type;
	}

}

sf::Packet & operator>>(sf::Packet & packet, OnlineRequest & o){
	int t;
	auto packet2 = packet >> t;
	o.type = (OnlineRequest::Type)t;
	if (t == OnlineRequest::KEY_EVENT) {
		auto packet3 = packet2 >> t;
		o.inputEvent.inputEventKey = (OnlineRequest::InputEventKey)t;
		return packet3 >> o.inputEvent.pressed >> o.inputEvent.leftPressed >> o.inputEvent.rightPressed >> o.inputEvent.downPressed >> o.inputEvent.boostPressed;
	}
	else if (o.type == OnlineRequest::CREATE || o.type == OnlineRequest::JOIN || o.type == OnlineRequest::RANDOM) {
		return packet2 >> o.stringParam;
	}
	else if (o.type == OnlineRequest::READY) {
		return packet2 >> o.stringParam >> o.intParam >> o.boolParam;
	}
	else if (o.type == OnlineRequest::CHARACTER_CHANGE) {
		int ci;
		auto packet3 = packet2 >> o.stringParam >> o.intParam >> ci;
		o.ciParam = (glb::characterIndex)ci;
		return packet3;
	}
	else {
		return packet2;
	}
}