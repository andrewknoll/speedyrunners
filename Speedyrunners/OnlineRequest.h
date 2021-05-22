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
		GET_CHAR
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
};

sf::Packet& operator<< (sf::Packet& packet, const OnlineRequest& o)
{
	if (o.type == OnlineRequest::KEY_EVENT) {
		return packet << (int)o.type << o.inputEvent.inputEventKey << o.inputEvent.pressed << o.inputEvent.leftPressed << o.inputEvent.rightPressed << o.inputEvent.downPressed << o.inputEvent.boostPressed;;
	}
	else if(o.type == OnlineRequest::CREATE || o.type == OnlineRequest::JOIN || o.type == OnlineRequest::RANDOM){
		return packet << (int)o.type << o.stringParam;
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
	else {
		return packet2;
	}
	
}

