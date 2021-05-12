#pragma once
#include <random>
#include "SFML/Audio.hpp"
class AudioPlayer
{
public:
	enum class Effect {
		JUMP,
		FOOTSTEP,
		FOLEY_LAND, 
		ROLL,
		SLIDE,
		PICKUP,
		SPEEDBOOST,
		DEATH,
		BOING, // yup
		CHARGE_GROUNDBOOST, // ? maybe the blue boost thing
		SPIKES_HIT,
		DOUBLE_JUMP,
		PLAYER_LAND,
		SLIDE_DOWN_LOOP,
		SLIDE_DOWN_START,
		SLIDE_DOWN_SKID,
		TACKLE,
		WIND_AIRTIME, // going fast in the air?
		WRONG_WAY,
		PICKUP_BOOSTACOKE,
		// ----------------- UI:
		GAMEPLAY_TIMERUP,
		UI_COUNTDOWN,
		CHARACTER_OUTOFSCREEN,
		COUNTDOWN_SLIDE_IN,
		COUNTDOWN_TIMER_1,
		COUNTDOWN_TIMER_3_2,
		COUNTDOWN_TIMER_GO,
		COUNTDOWN_TIMER_BUILDUP,
		PAUSE,
		SUDDEN_DEATH_ACTIVATE,
		WIN_GOTO_SCOREBOARD,
		WIN_ROUND_1,
		WIN_ROUND_2,
		TEXT_APPEAR,
		TEXT_DISAPPEAR
	};
protected:
	std::vector<std::vector<sf::Sound>> sounds; // like sf::Sprite for sounds

	std::mt19937 sampler { std::random_device{}() }; 


	sf::Sound& getSample(const int effect); // Returns a random sample from the effect type

public:
	AudioPlayer();
	AudioPlayer(const std::vector<std::vector<sf::SoundBuffer>>& buffers);

	void loadSoundsFromBuffers(const std::vector<std::vector<sf::SoundBuffer>>& buffers);

	sf::Sound& getSample(const Effect effect); // Returns a random sample from the effect type

	// Play from beggining once
	void play(const int effect);
	void play(const Effect effect);

	// Loop (or stop looping if loopIt=false)
	void loop(const int effect, bool loopIt = true);
	void loop(const Effect effect, bool loopIt = true);

	// Stop all efects from the given type
	void stop(const int effect);
	void stop(const Effect effect);


};

