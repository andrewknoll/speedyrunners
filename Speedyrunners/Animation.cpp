#include "Animation.h"
bool Animation::insert(int where, sf::IntRect new_frame) {
	int i = 0;
	while (i < frame_numbers.size() && frame_numbers[i] < where) {
		if (frame_numbers[i] == where) {
			return false;
		}
		else {
			frames[i] = new_frame;
		}
	}
	return true;
}