#include "AudioPlayer.h"

#include "Resources.h"
#include <cassert>

AudioPlayer::AudioPlayer() : buffers(Resources::getInstance().getSoundBuffers())
{
	auto& bufs = Resources::getInstance().getSoundBuffers(); 
	for (const auto& buffer : bufs) {
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


