#include "MusicPlayer.h"


//#define DISABLE_MUSIC // DEBUG jeje

MusicPlayer::MusicPlayer():
	tracks(std::vector<std::vector<std::shared_ptr<sf::Music> > >(n_types)),
	musicSampler(std::vector<rng::Sampler< std::shared_ptr<sf::Music> > >())
{
}

void MusicPlayer::setUp() {
	for (std::vector<std::shared_ptr<sf::Music> >& t : tracks) {
		if (!t.empty()) musicSampler.emplace_back(t);
		for (std::shared_ptr<sf::Music>& ptr : t) {
			ptr->setVolume(volume);
			ptr->setLoop(true);
		}
	}
}

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
				tracks[i][j]->play();
			}
		}
	}
#endif
}

void MusicPlayer::playMusicTrack(MusicType t, int variant) {
	playMusicTrack((int)t, variant);
}

MusicPlayer::MusicType MusicPlayer::selectRandomTrack(MusicType t){
	selected = musicSampler[(int)t].getSample();
	return t;
}

MusicPlayer::MusicType MusicPlayer::selectRandomTrack(const std::vector<MusicType>& t) {
	int n = -1;
	for (const MusicType& i : t) {
		n += tracks[(int)i].size();
	}
	int r = rng::defaultGen.rand(0, n);
	for (const MusicType& i : t) {
		if (r < tracks[(int)i].size()) {
			selected = musicSampler[(int)i].getSample();
			selectedValid = true;
			return i;
		}
	}
}

void MusicPlayer::playSelected() {
	stopAll();
	selected->play();
	selectedValid = false;
}

bool MusicPlayer::hasSelected() const {
	return selectedValid;
}

void MusicPlayer::pauseAll() {
	for (int i = 0; i < n_types; i++) {
		for (int j = 0; j < tracks[i].size(); j++) {
			tracks[i][j]->pause();
		}
	}
}

void MusicPlayer::stopAll() {
	for (int i = 0; i < n_types; i++) {
		for (int j = 0; j < tracks[i].size(); j++) {
			tracks[i][j]->stop();
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
