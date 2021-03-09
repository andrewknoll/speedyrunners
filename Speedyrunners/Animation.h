#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <memory>
class Animation
{
private:
	std::shared_ptr<sf::Texture> spritesheet;
	std::vector<int> frame_numbers;
	std::vector<sf::IntRect> frames;
	int current_frame = 0;
	sf::Vector2f position;
public:
	sf::Vector2f get_position();
	void set_position(sf::Vector2f new_position);
	void set_spritesheet(std::shared_ptr<sf::Texture> spritesheet);
	bool insert(int where, sf::IntRect new_frame);
	sf::Sprite get_first_frame(bool reverse);
	int advance_frame(sf::Sprite& sprite, bool loop, bool reverse);
	int get_current_frame();
	sf::Vector2f get_center_offset();
};

