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
public:
	void set_spritesheet(std::shared_ptr<sf::Texture> spritesheet);
	bool insert(int where, sf::IntRect new_frame);
	sf::Sprite get_first_frame();
	int advance_frame(sf::Sprite& sprite);
	int get_current_frame();
};

