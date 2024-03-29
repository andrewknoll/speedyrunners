#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <memory>
class Animation : protected sf::Transformable
{
private:
	std::shared_ptr<const sf::Texture> spritesheet;
	std::vector<int> frame_numbers;
	std::vector<sf::IntRect> frames;
	int current_frame = 0;
	bool loop = false;
	bool reverse = false;
	bool facing_right = true;
	bool finished = false;
public:
	//Animation(const std::vector<sf::IntRect>& _frames, sf::Texture& tex);
	Animation();
	Animation(const sf::Vector2i& nFrames, const sf::Texture& tex); // animation for <tex> texture, assuming the frames are in a grid of nFrames.x by nFrames.y
	Animation(int x, int y, const sf::Texture& tex); // animation for <tex> texture, assuming the frames are in a grid of x by y

	void set_spritesheet(std::shared_ptr<const sf::Texture> spritesheet);
	bool insert(int where, sf::IntRect new_frame);
	sf::Sprite get_first_frame();
	void flip(sf::Sprite& sprite);
	int advance_frame(sf::Sprite& sprite);
	int get_current_frame() const;
	sf::Vector2f get_origin_point();
	void set_loop(bool loop);
	bool get_loop() const;
	void set_reverse(bool reverse);
	bool get_reverse() const;
	void update_orientation(bool right);
	void update_angle(sf::Sprite& sp, float angle);
	bool getFinished() const;
	void set_scale(float x, float y);
};

