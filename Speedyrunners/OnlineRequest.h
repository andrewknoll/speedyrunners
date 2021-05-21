#pragma once
#include "SFML/Network.hpp"
#include <variant>

struct OnlineRequest
{
	enum Type {
		CREATE,
		JOIN,
		RANDOM,
		KEY_EVENT
	};

	enum InputEventKey {
		NONE,
		JUMP,
		HOOK,
		ITEM
	};
	struct InputEvent {
		InputEventKey inputEventKey = NONE;
		bool pressed = false;
		bool leftPressed = false;
		bool rightPressed = false;
		bool downPressed = false;
		bool boostPressed = false;
	};
	Type type = CREATE;
	std::string stringParam = "";
	InputEvent inputEvent;
};

sf::Packet& operator<< (sf::Packet& packet, const OnlineRequest& o)
{
	if (o.type == OnlineRequest::KEY_EVENT) {
		return packet << (int)o.type << o.inputEvent.inputEventKey << o.inputEvent.pressed << o.inputEvent.leftPressed << o.inputEvent.rightPressed << o.inputEvent.downPressed << o.inputEvent.boostPressed;;
	}
	else {
		return packet << (int)o.type << o.stringParam;
	}
	
}
sf::Packet& operator>> (sf::Packet& packet, OnlineRequest& o)
{
	int t;
	auto packet2 = packet >> t;
	o.type = (OnlineRequest::Type)t;
	if (t == OnlineRequest::KEY_EVENT) {
		auto packet3 = packet2 >> t;
		o.inputEvent.inputEventKey = (OnlineRequest::InputEventKey)t;
		return packet3 >> o.inputEvent.pressed >> o.inputEvent.leftPressed >> o.inputEvent.rightPressed >> o.inputEvent.downPressed >> o.inputEvent.boostPressed;
	}
	else {
		return packet2 >> o.stringParam;
	}
	
}

