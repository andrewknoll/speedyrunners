#pragma once
#include <SFML/Graphics.hpp>
#include <string>
namespace glb
{
	const double pi = 3.14159265358979323846;
	const int NUMBER_OF_ANIMATIONS = 22;
	namespace anim {
		const std::string STAND_ANIM = "Stand";
		const std::string RUN_ANIM = "Run";
		const std::string SLIDE_ANIM = "Slide";
		const std::string SLIDING_ANIM = "Sliding";
		const std::string FALL_ANIM = "Straight-Fall";
		const std::string JUMP_ANIM = "Straight-Jump";
		const std::string LONG_FALL_ANIM = "Long-Fall";
		const std::string LONG_JUMP_ANIM = "Long-Jump";
		const std::string D_JUMP_ANIM = "Double-Jump";
		const std::string D_JUMP_FALL_ANIM = "Double-Jump-Fall";
		const std::string HOOK_SHOT_ANIM = "Hookshot";
		const std::string SWING_ANIM = "Swing";
		const std::string RUNNING_HOOK_ANIM = "Running-Hook";
		const std::string WALL_HANG_ANIM = "Wall-Hang";
		const std::string FLIP_ANIM = "Flip";
		const std::string ROLL_ANIM = "Roll";
		const std::string TUMBLE_ANIM = "Tumble";
		const std::string GRABBED_ANIM = "Grabbed";
		const std::string SKID_ANIM = "Skid";
		const std::string SPIKED_ANIM = "Spiked";
		const std::string TAUNT_ANIM = "Taunt";
		const std::string START_ANIM = "321GO";
	};
	const int NUMBER_OF_ITEMS = 1;
	enum item : int {
		NONE = 0,
		ROCKET = 1
	};

	const sf::Rect<float> default_hitbox = sf::Rect<float>(20,20,14, 30);

	const float runningAcceleration = 800;
	const float flyingAcceleration = 400;
	const float jumpingSpeed = 200;

	const sf::Vector2f tileSize = sf::Vector2f(18.0, 18.0);

	const float viewMarginFactor = 0.94;

	const float cameraZoomFunctionSteepness = 1e-4;



	// Paths:
	const std::string CONTENT_PATH = "../assets/Content/";

	const std::string PASSPORT_PATH = CONTENT_PATH + "UI/Ingame/Passports/";
};

