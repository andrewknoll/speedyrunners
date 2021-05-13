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
	for (const auto& bufs : buffers) {
		sounds.emplace_back(); // new inside vector (e.g. for the 4 jump sounds)
		auto& lastVec = sounds.back();
		for (const auto& buffer : bufs) {
			lastVec.emplace_back(buffer); // one of the sounds
		}
	}
}

sf::Sound& AudioPlayer::getSample(const Effect effect)
{
	return getSample((int)effect);
}

sf::Sound& AudioPlayer::getSample(const int effect) {
	assert(effect < sounds.size());
	// Pick random sample from the appropiate type:
	std::uniform_int_distribution<> distr(0, sounds[effect].size() - 1); // define the range (-1 because its inclusive)
	int sample = distr(sampler);
	return sounds[effect][sample];
}

void AudioPlayer::play(const int effect)
{
	getSample(effect).play();
}

void AudioPlayer::play(const Effect effect)
{
	play((int)effect);
}


void AudioPlayer::loop(const Effect effect, bool loopIt)
{
	setLoop(effect, loopIt);
	continuePlaying(effect);
}

void AudioPlayer::setLoop(const Effect effect, bool loopIt)
{	
	for (auto& sound : sounds[(int)effect])
		sound.setLoop(loopIt);
}

void AudioPlayer::continuePlaying(const Effect effect)
{
	auto& s = getSample(effect);
	if (s.getStatus() != sf::Sound::Status::Playing) s.play();
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

