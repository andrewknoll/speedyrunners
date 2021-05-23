#pragma once
#include <SFML/Audio.hpp>
#include "utils.hpp"
class MusicPlayer
{
public:
	enum class MusicType : int { MENU = 0, REGULAR = 1, SUDDENDEATH = 2, REGULAR_WITH_PAUSE = 3, EDIT_MODE = 4};
private:
	const int n_types = 5;
	std::vector<std::vector<std::shared_ptr<sf::Music> > > tracks;
	float volume = 30; // between 0 and 100
	std::vector<rng::Sampler< std::shared_ptr<sf::Music> > > musicSampler;
	std::shared_ptr<sf::Music> selected;
	bool selectedValid = false;
public:
	MusicPlayer();
	void setUp();
	void addTrack(std::string file, int t);
	void addTrack(std::string file, MusicType t);

	void playMusicTrack(int t, int variant = 0);
	void playMusicTrack(MusicType t, int variant = 0);
	MusicType selectRandomTrack(MusicType t);
	MusicType selectRandomTrack(const std::vector<MusicType>& t);
	void playSelected();
	bool hasSelected() const;

	void pauseAll();
	void stopAll();

	bool isPlaying(int t);
	bool isPlaying(MusicType t);

	void setVolume(float v);
	float getVolume() const;
};

