#pragma once
#include <SFML/Audio.hpp>
class MusicPlayer
{
public:
	enum MusicType : int { MENU = 0, REGULAR = 1, SUDDENDEATH = 2 };
private:
	const int n_tracks = 3;
	std::vector<sf::Music> tracks = std::vector<sf::Music>(n_tracks);
	
public:
	void loadMusicFile(std::string f, MusicType t);

	void playMusicTrack(MusicType t);

	void pauseAll();

	bool isPlaying(MusicType t);
};

