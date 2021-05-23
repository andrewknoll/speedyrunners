#pragma once
#include <SFML/Audio.hpp>
#include "utils.hpp"
class MusicPlayer
{
public:
	enum class MusicType : int { MENU = 0, REGULAR = 1, SUDDENDEATH = 2 };
private:
	const int n_types = 3;
	std::vector<std::vector<std::shared_ptr<sf::Music> > > tracks;
	float volume = 30; // between 0 and 100
	std::vector<rng::Sampler< std::shared_ptr<sf::Music> > > musicSampler;
public:
	MusicPlayer();
	void generateSamplers();
	void addTrack(std::string file, int t);
	void addTrack(std::string file, MusicType t);

	void playMusicTrack(int t, int variant = 0);
	void playMusicTrack(MusicType t, int variant = 0);
	void playRandomTrack(MusicType t);

	void pauseAll();
	void stopAll();

	bool isPlaying(int t);
	bool isPlaying(MusicType t);

	void setVolume(float v);
	float getVolume() const;
};

