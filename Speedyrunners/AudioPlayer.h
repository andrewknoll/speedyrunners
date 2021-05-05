#pragma once
#include <random>
#include "SFML/Audio.hpp"
class AudioPlayer
{
public:
	// TODO: Varios sounds por efecto (Jump tiene 4) (vector de vectores mismo). Colocar cada efecto en su sitio 
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
		PICKUP_BOOSTACOKE
	};
protected:
	std::vector<std::vector<sf::Sound>> sounds; // like sf::Sprite for sounds

	std::mt19937 sampler { std::random_device{}() }; 

public:
	AudioPlayer();
	AudioPlayer(const std::vector<std::vector<sf::SoundBuffer>>& buffers);

	void loadSoundsFromBuffers(const std::vector<std::vector<sf::SoundBuffer>>& buffers);

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

