#pragma once
#include "SFML/Network.hpp"
#include <variant>
#include "Globals.hpp"
#include "Settings.h"

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

sf::Packet& operator<< (sf::Packet& packet, const OnlineRequest& o);
sf::Packet& operator>> (sf::Packet& packet, OnlineRequest& o);

