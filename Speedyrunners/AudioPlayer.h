#pragma once
#include <random>
#include "SFML/Audio.hpp"
class AudioPlayer
{
public:
	// TODO: Varios sounds por efecto (Jump tiene 4) (vector de vectores mismo). Colocar cada efecto en su sitio 
	enum class Effect {
		JUMP,
		LAND,
		SLIDE
	};
protected:
	std::vector<std::vector<sf::Sound>> sounds; // like sf::Sprite for sounds

	std::mt19937 sampler { std::random_device{}() }; 

public:
	AudioPlayer();
	AudioPlayer(const std::vector<std::vector<sf::SoundBuffer>>& buffers);

	void loadSoundsFromBuffers(const std::vector<std::vector<sf::SoundBuffer>>& buffers);

	void play(const int effect);

	void play(const Effect effect);

};

