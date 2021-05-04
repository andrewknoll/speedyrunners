#pragma once

#include "SFML/Audio.hpp"
class AudioPlayer
{
public:
	enum class Effect {
		JUMP,
		LAND,
		SLIDE
	};
protected:
	const std::vector<sf::SoundBuffer>& buffers; // like sf::Textures for small (not music) sounds
	std::vector<sf::Sound> sounds; // like sf::Sprite


public:
	AudioPlayer();

	void play(const int effect);

	void play(const Effect effect);

};

