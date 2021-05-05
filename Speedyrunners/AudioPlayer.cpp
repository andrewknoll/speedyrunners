#include "AudioPlayer.h"

#include "Resources.h"
#include <cassert>

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::AudioPlayer(const std::vector<std::vector<sf::SoundBuffer>>& buffers)
{
	loadSoundsFromBuffers(buffers);
}

void AudioPlayer::loadSoundsFromBuffers(const std::vector<std::vector<sf::SoundBuffer>>& buffers)
{
	//auto& bufs = Resources::getInstance().getSoundBuffers();
	for (const auto& bufs : buffers) {
		sounds.emplace_back(); // new inside vector (e.g. for the 4 jump sounds)
		auto& lastVec = sounds.back();
		for (const auto& buffer : bufs) {
			lastVec.emplace_back(buffer); // one of the sounds
		}
	}
}

void AudioPlayer::play(const int effect)
{
	assert(effect < sounds.size());
	// Pick random sample from the appropiate type:
	std::uniform_int_distribution<> distr(0, sounds[effect].size()-1); // define the range (-1 because its inclusive)
	int sample = distr(sampler);
	sounds[effect][sample].play();
}

void AudioPlayer::play(const Effect effect)
{
	play((int)effect);
}

void AudioPlayer::loop(const int effect, bool loopIt)
{
	assert(effect < sounds.size());
	// Pick random sample from the appropiate type:
	std::uniform_int_distribution<> distr(0, sounds[effect].size() - 1); // define the range (-1 because its inclusive)
	int sample = distr(sampler);
	sounds[effect][sample].setLoop(loopIt);
}

void AudioPlayer::loop(const Effect effect, bool loopIt)
{
	loop((int)effect, loopIt);
}

void AudioPlayer::stop(const int effect)
{
	assert(effect < sounds.size());
	for (auto& sound : sounds[effect]) sound.stop();
}

void AudioPlayer::stop(const Effect effect)
{
	stop((int)effect);
}

