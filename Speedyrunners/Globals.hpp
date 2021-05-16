#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <array>
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
	const int NUMBER_OF_MISC_TEXTURES = 11;
	const sf::Vector2f FEET_TO_HAND = sf::Vector2f(20.0, 0);// -50.0
	// Items:
	const int NUMBER_OF_ITEMS = 10;
	const int NUMBER_OF_UNOBTAINABLE_ITEMS = 3;
	enum item : int {
		NONE = 0,
		ROCKET = 1,
		BOULDER, // rolling thing
		FREEZE,
		GOLDEN_HOOK,
		INVINCI_DRILL,
		TNT,
		CRATE_3,
		// these 3 must be last!!:
		TNT_DETONATOR,
		CRATE_2,
		CRATE,
	};

	// For InGameUI, returns the index in the UI/Sprites/MultiplayerHUD/Powerups.png texture:
	const std::array<int, NUMBER_OF_ITEMS+1> itemToTexIndex { {
		0, // NONE is first (hay un hueco al principio)
		4,  // Rocket is 5th
		15, // rolling thing
		16, // freeze
		1, // golden hook
		3, // invinci drill
		5, // TNT
		9,// Crates
		6, // TNT detonator
		8,
		2 
	} };

	// Characters:
	enum characterIndex : int {
		SPEEDRUNNER = 0,
		COSMONAUT = 1,
		UNIC = 2,
		FALCON = 3
	};

	// Cuidado cambiando esto, se usan para paths (en Character::Character):
	const std::vector<std::string> characterNames{ "Speedrunner", "Cosmonaut", "Unic", "Falcon" };


	const sf::Time STUN_TIME = sf::seconds(1); // Rockets
	const sf::Time TUMBLE_TIME = sf::seconds(0.5); // box obstacles



	const sf::Time itemPickupRespawn = sf::seconds(10); // Respawn time for the capsule item thingies
	const sf::Time LVL_ANIMATION_PERIOD = sf::seconds(0.05);


	const float runningAcceleration = 800;
	const float flyingAcceleration = 350;
	const float jumpingSpeed = 175;

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

	const std::string BG_PATH = CONTENT_PATH + "Backgrounds/";
	const int N_BACKGROUNDS = 8;
	const std::vector<std::string> bgPaths { 
			BG_PATH + "ENV_Zoo/bg_1280x720.png",
			BG_PATH + "ENV_Airport/sky_1280_720.png",
			BG_PATH + "ENV_Boostacoke/Sky.png",
			BG_PATH + "ENV_Casino/Sky.png",
			BG_PATH + "ENV_Mansion/Sky.png",
			BG_PATH + "ENV_NightClub/bg_1280_720.png",
			BG_PATH + "ENV_Ski/bg_sky.png",
			BG_PATH + "ENV_ThemePark/sky_1280x720.png"
		};


	// particlesystems:
	enum particleSystemIdx {
		ROCKET_SMOKE,
		ROCKET_CLOUD,
		BOOST,
		BRAKE // when sliding, etc
	};

	const float tumbleSpeedReduction = 0.85f;

	inline const bool enableRandomObstacles = true;
};

