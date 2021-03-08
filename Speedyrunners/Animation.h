#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
class Animation
{
private:
	std::vector<int> frame_numbers;
	std::vector<sf::IntRect> frames;
public:
	bool insert(int where, sf::IntRect new_frame);
};

