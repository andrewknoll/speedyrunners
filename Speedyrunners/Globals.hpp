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
	const int NUMBER_OF_MISC_TEXTURES = 5;
	const sf::Vector2f FEET_TO_HAND = sf::Vector2f(20.0, 0);// -50.0);
	enum item : int {
		NONE = 0,
		ROCKET = 1
	};
	enum characterIndex : int {
		SPEEDRUNNER = 0,
		COSMONAUT = 1,
		UNIC = 2,
		FALCON = 3
	};

	// Cuidado cambiando esto, se usan para paths (en Character::Character):
	const std::vector<std::string> characterNames{ "Speedrunner", "Cosmonaut", "Unic", "Falcon" };


	const float runningAcceleration = 800;
	const float flyingAcceleration = 350;
	const float jumpingSpeed = 150;

	const sf::Vector2f tileSize = sf::Vector2f(18.0, 18.0);

	const sf::Rect<float> default_hitbox = sf::Rect<float>(20, 20, tileSize.x, tileSize.y * 2.0f);


	const float viewMarginFactor = 0.8;

	const float cameraZoomFunctionSteepness = 1e-4;


	const sf::Vector2i jump1Tiles = { 10,3 };
	const sf::Vector2i jump2Tiles = { 10,3 };

	const float cosine45 = 0.70710678118654752440f;

	// Paths:
	const std::string CONTENT_PATH = "../assets/Content/";
	const std::string LEVELS_PATH = "../assets/levels/";

	const std::string PASSPORT_PATH = CONTENT_PATH + "UI/Ingame/Passports/";
};

