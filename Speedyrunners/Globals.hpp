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
	const int NUMBER_OF_ITEMS = 8;
	const int NUMBER_OF_UNOBTAINABLE_ITEMS = 3;
	enum item : int {
		NONE = 0,
		ROCKET = 1,
		ICERAY = 2,
		GOLDEN_HOOK,
		TNT,
		CRATE_3,
		// these 3 must be last!!:
		CRATE_2,
		CRATE,
		TNT_DETONATOR
	};

	// For InGameUI, returns the index in the UI/Sprites/MultiplayerHUD/Powerups.png texture:
	const std::array<int, NUMBER_OF_ITEMS+1> itemToTexIndex { {
		0, // NONE is first (hay un hueco al principio)
		4,  // Rocket is 5th
		16, // freeze
		1, // golden hook¡
		5, // TNT
		9, // Crates
		8,
		2,
		6 // TNT detonator
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
	const sf::Time FREEZE_TIME = sf::seconds(5);
	const sf::Time TUMBLE_TIME = sf::seconds(0.5); // box obstacles
	const sf::Time FROZEN_WIGGLE_CD = sf::seconds(0.3);
	const sf::Time ANIMATION_PERIOD = sf::milliseconds(1000 / 30);

	const int WIGGLE_FRAMES = 6;

	const sf::Time itemPickupRespawn = sf::seconds(10); // Respawn time for the capsule item thingies
	const sf::Time LVL_ANIMATION_PERIOD = sf::seconds(0.05);
	const float SUPERSPEED_THRESHOLD = 0.95; // min speed (relative to physics::FALL_SPEED) to get superspeed

	const float runningAcceleration = 800;
	const float flyingAcceleration = 350;
	const float jumpingSpeed = 175*1.45f;

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
	const std::string SETTINGS_PATH = "../assets/settings.csv";

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

	//MiscTextures:
	enum miscTextureIdx {
		HOOK_TEX,
		SCORE_POINT_TEX,
		WINNER_LABEL_TEX,
		BOOST_SECTION_TEX,
		BOOST_BAR_TEX,
		POWER_UP_TEX,
		POWER_UP_HUD_BG_TEX,
		POWER_UP_HUD_TEX,
		SMOKE_P_TEX,
		CLOUD_P_TEX,
		PLUS_P_TEX,
		BRAKE_P_TEX,
		OBSTACLES_TEX,
		BOMB_TEX,
		SUPER_BOOST_TEX,
		FIRE_P_TEX,
		TRAIL_TEX,
		ICE_CUBE_TEX,
		ICE_BEAM
	};

	// particlesystems:
	enum particleSystemIdx {
		ROCKET_SMOKE,
		ROCKET_CLOUD,
		BOOST,
		BRAKE, // when sliding, etc
		SUPER_SPEED,
		SUDDEN_DEATH_EXPLOSION,
		ICE_BEAM_PS
	};

	const float tumbleSpeedReduction = 0.85f;

	inline const bool enableRandomObstacles = true;
};

