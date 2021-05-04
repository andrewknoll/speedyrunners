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
	std::vector<sf::Sound> sounds; // like sf::Sprite for sounds

public:
	AudioPlayer();
	AudioPlayer(const std::vector<sf::SoundBuffer>& buffers);

	void loadSoundsFromBuffers(const std::vector<sf::SoundBuffer>& buffers);

	void play(const int effect);

	void play(const Effect effect);

};

