#include "Animation.h"
void Animation::set_spritesheet(std::shared_ptr<sf::Texture> spritesheet) {
	this->spritesheet = spritesheet;
}

bool Animation::insert(int where, sf::IntRect new_frame) {
	std::vector<int>::iterator it_num = frame_numbers.begin();
	std::vector<sf::IntRect>::iterator it_rect = frames.begin();
	if (frame_numbers.empty()) {
		frame_numbers.insert(it_num, where);
		frames.insert(it_rect, new_frame);
	}
	else {
		while (it_num < frame_numbers.end() && *it_num < where) {
			it_num++;
			it_rect++;
		}
		if (it_num != frame_numbers.end()) {
			if (*it_num == where) return false;
		}
		frame_numbers.insert(it_num, where);
		frames.insert(it_rect, new_frame);
	}
	return true;
}

sf::Sprite Animation::get_first_frame() {
	sf::Sprite sprite;
	sprite.setTexture(*spritesheet);
	sprite.setTextureRect(frames[0]);
	return sprite;
}

//Returns 1 when animation was reset
int Animation::advance_frame(sf::Sprite& sprite) {
	sf::Sprite new_sprite;
	new_sprite.setTexture(*spritesheet);
	if (current_frame < frames.size() -1) {
		new_sprite.setTextureRect(frames[++current_frame]);
		sprite = new_sprite;
		return 0;
	}
	else {
		current_frame = 0;
		new_sprite.setTextureRect(frames[0]);
		sprite = new_sprite;
		return 1;
	}
}

int Animation::get_current_frame() {
	return current_frame;
}

