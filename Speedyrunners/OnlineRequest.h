#pragma once
#include "SFML/Network.hpp"
#include <variant>

struct OnlineRequest
{
	enum Type {
		CREATE,
		JOIN,
		RANDOM,
		KEY_EVENT,
		GET_PS,
		GET_IT,
		GET_RV,
		GET_CD,
		GET_CAM,
		GET_LVL,
		GET_CHP,
		GET_NPC,
		GET_CHAR,
		GET_ST,
		READY,
		CHARACTER_CHANGE
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
	std::shared_ptr<Settings> settings;
	InputEvent inputEvent;
	int intParam = -1;
	bool boolParam = false;
	glb::characterIndex ciParam = glb::characterIndex::SPEEDRUNNER;
};

sf::Packet& operator<< (sf::Packet& packet, const OnlineRequest& o)
{
	if (o.type == OnlineRequest::KEY_EVENT) {
		return packet << (int)o.type << o.inputEvent.inputEventKey << o.inputEvent.pressed << o.inputEvent.leftPressed << o.inputEvent.rightPressed << o.inputEvent.downPressed << o.inputEvent.boostPressed;;
	}
	else if(o.type == OnlineRequest::CREATE || o.type == OnlineRequest::JOIN || o.type == OnlineRequest::RANDOM){
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
	else if (o.type == OnlineRequest::CREATE || o.type == OnlineRequest::JOIN || o.type == OnlineRequest::RANDOM){
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

