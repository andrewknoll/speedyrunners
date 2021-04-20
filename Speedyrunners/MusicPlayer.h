#pragma once
#include <SFML/Audio.hpp>
class MusicPlayer
{
public:
	enum class MusicType : int { MENU = 0, REGULAR = 1, SUDDENDEATH = 2 };
private:
	const int n_types = 3;
	std::vector<std::vector<std::shared_ptr<sf::Music> > > tracks = std::vector<std::vector<std::shared_ptr<sf::Music> > >(n_types);
	
public:
	void addTrack(std::string file, int t);
	void addTrack(std::string file, MusicType t);

	void playMusicTrack(int t, int variant = 0);
	void playMusicTrack(MusicType t, int variant = 0);

	void pauseAll();

	bool isPlaying(int t);
	bool isPlaying(MusicType t);
};

