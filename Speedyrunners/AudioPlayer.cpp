#include "AudioPlayer.h"

#include "Resources.h"
#include <cassert>

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::AudioPlayer(const std::vector<sf::SoundBuffer>& buffers)
{
	loadSoundsFromBuffers(buffers);
}

void AudioPlayer::loadSoundsFromBuffers(const std::vector<sf::SoundBuffer>& buffers)
{
	//auto& bufs = Resources::getInstance().getSoundBuffers();
	for (const auto& buffer : buffers) {
		sounds.emplace_back(buffer);
	}
}

void AudioPlayer::play(const int effect)
{
	assert(effect < sounds.size());
	sounds[effect].play();
}

void AudioPlayer::play(const Effect effect)
{
	play((int)effect);
}


