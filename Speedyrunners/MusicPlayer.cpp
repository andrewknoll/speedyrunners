#include "MusicPlayer.h"

void MusicPlayer::loadMusicFile(std::string f, MusicType t) {
	tracks[t].openFromFile(f);
}

void MusicPlayer::playMusicTrack(MusicType t) {
	for (int i = 0; i < n_tracks; i++) {
		if(i != t) tracks[i].stop();
		else tracks[i].play();
	}
}

void MusicPlayer::pauseAll() {
	for (int i = 0; i < n_tracks; i++) {
		tracks[i].pause();
	}
}

bool MusicPlayer::isPlaying(MusicType t) {
	return tracks[t].getStatus() == sf::Music::Playing;
}