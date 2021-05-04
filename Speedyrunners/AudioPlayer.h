#pragma once

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
	std::vector<sf::Sound> sounds; // like sf::Sprite for sounds

public:
	AudioPlayer();
	AudioPlayer(const std::vector<sf::SoundBuffer>& buffers);

	void loadSoundsFromBuffers(const std::vector<sf::SoundBuffer>& buffers);

	void play(const int effect);

	void play(const Effect effect);

};

