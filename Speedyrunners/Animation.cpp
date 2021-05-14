#include "Animation.h"
void Animation::set_spritesheet(std::shared_ptr<const sf::Texture> spritesheet) {
	this->spritesheet = spritesheet;
}


Animation::Animation(const sf::Vector2i& nFrames, const sf::Texture& tex) : spritesheet(std::make_shared<sf::Texture>(tex))
{
	loop = true;
	sf::Vector2i size(tex.getSize().x / nFrames.x, tex.getSize().y / nFrames.y); // size of each frame
	for (int col = 0; col < nFrames.y; col++)
		for (int row = 0; row < nFrames.x; row++)
			frames.emplace_back(row * size.x, col * size.y, size.x, size.y);
}

Animation::Animation(int x, int y, const sf::Texture& tex) : Animation(sf::Vector2i(x,y),tex)
{
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
	sprite.setTextureRect(frames[reverse? frames.size() - 1 : 0]);
	current_frame = reverse ? frames.size() - 1 : 0;
	//sprite.setPosition(this->get_position() - this->get_center_offset());
	sprite.setOrigin(this->get_origin_point());
	//sprite.setOrigin(sprite.getOrigin());
	flip(sprite);
	sprite.setRotation(angle);
	return sprite;
}


void Animation::flip(sf::Sprite& sprite) {
	//sprite.setOrigin(this->get_origin_point());
	if (!facing_right && sprite.getScale().x > 0 
		|| facing_right && sprite.getScale().x < 0) {
		sprite.scale(-1.0, 1.0);
	}
	/*else if (facing_right && sprite.getScale().x < 0) {
		sprite.scale(1.0, 1.0);
	}*/
}
// return 1 if last frame
int Animation::advance_frame(sf::Sprite& sprite) {
	int return_code = 0;
	if (!spritesheet) return 0;
	sprite.setTexture(*spritesheet);
	if (!reverse) {
		if (current_frame < frames.size() - 1) {
			sprite.setTextureRect(frames[++current_frame]);
		}
		else if (loop) {
			current_frame = 0;
			sprite.setTextureRect(frames[0]);
		}
		if (current_frame == frames.size() - 1) return_code = 1;
	}
	else {
		if (current_frame > 0) {
			sprite.setTextureRect(frames[--current_frame]);
		}
		else if (loop) {
			current_frame = frames.size() - 1;
			sprite.setTextureRect(frames[current_frame]);
		}
		if (current_frame == 0) return_code = 1;
	}
	flip(sprite);
	sprite.setRotation(angle);
	return return_code;
}

int Animation::get_current_frame() {
	return current_frame;
}

sf::Vector2f Animation::get_origin_point() {
	sf::Vector2f v;
	v.x = frames[current_frame].width / 2.0;
	v.y = frames[current_frame].height;
	return v;
}

Animation::Animation()
{
}


sf::Vector2f Animation::get_position() {
	return position;
}

void Animation::setPosition(sf::Vector2f new_position) {
	position = new_position;
}

void Animation::setPosition(float x, float y) {
	position = sf::Vector2f(x, y);
}

void Animation::set_loop(bool loop) {
	this->loop = loop;
}

bool Animation::get_loop() const {
	return loop;
}

void Animation::set_reverse(bool reverse) {
	this->reverse = reverse;
}

bool Animation::get_reverse() const {
	return reverse;
}

void Animation::update_orientation(bool right) {
	facing_right = right;
}

void Animation::update_angle(sf::Sprite& sp, float angle) {
	this->angle = angle;
	sp.setRotation(angle);
}

