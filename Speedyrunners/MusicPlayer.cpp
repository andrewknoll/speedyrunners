#include "MusicPlayer.h"


//#define DISABLE_MUSIC // DEBUG jeje

void MusicPlayer::addTrack(std::string file, int t) {
	std::shared_ptr<sf::Music> m = std::make_shared<sf::Music>();
	m->openFromFile(file);
	tracks[t].push_back(m);
}

void MusicPlayer::addTrack(std::string file, MusicType t) {
	addTrack(file, (int)t);
}


void MusicPlayer::playMusicTrack(int t, int variant) {
#ifndef DISABLE_MUSIC
	for (int i = 0; i < n_types; i++) {
		for (int j = 0; j < tracks[i].size(); j++) {
			if (i != t || j != variant) tracks[i][j]->stop();
			else {
				tracks[i][j]->setVolume(volume);
				tracks[i][j]->setLoop(true);
				tracks[i][j]->play();
			}
		}
	}
#endif
}

void MusicPlayer::playMusicTrack(MusicType t, int variant) {
	playMusicTrack((int)t, variant);
}

void MusicPlayer::pauseAll() {
	for (int i = 0; i < n_types; i++) {
		for (int j = 0; j < tracks[i].size(); j++) {
			tracks[i][j]->pause();
		}
	}
}

bool MusicPlayer::isPlaying(int t) {
	bool any = false;
	int i = 0;
	while (!any && i < tracks[t].size()) {
		any |= tracks[t][i++]->getStatus() == sf::Music::Playing;
	}
	return any;
}

bool MusicPlayer::isPlaying(MusicType t) {
	return isPlaying((int)t);
}

void MusicPlayer::setVolume(float v)
{
	volume = v;
}

float MusicPlayer::getVolume() const
{
	return volume;
}
