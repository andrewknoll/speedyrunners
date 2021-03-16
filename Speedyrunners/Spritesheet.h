#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <map>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <memory>
class Spritesheet
{
private:
	enum returnCodes :int {
		Finished = 1,
		DataFileMissing = -1,
		ImageFileMissing = -2,
		UnrecognizedCharacter = -3,
		MissingCurlyBrace = -4,
		MissingBracket = -5,
		MissingFrames = -6,
		UnexpectedToken = -7,
		DuplicatedFilenameToken = -8,
		DuplicatedPosition = -9,
		DuplicatedFrame = -10,
		InvalidPosition = -11
	};

	enum flagsIndexes : int {
		GlobalScope = 0,
		FramesToken = 1,
		FramesScope = 2,
		SingleFrameScope = 3,
		FilenameToken = 4,
		FilenameRead = 5,
		PositionToken = 6,
		PositionScope = 7,
		UnusedSubscope = 8,
		Position_X = 9,
		Position_Y = 10,
		Position_W = 11,
		Position_H = 12,
		MetaScope = 13,
		MetaSubscope = 14
	};

	std::map<std::string, Animation> animations;
	bool flags[15];
	std::shared_ptr<sf::Texture> texture;

	bool remove_commas_or_spaces(std::string& matched);
	int process_token(std::string matched);
public:
	int parse_spritesheet(std::string image_filename, std::string data_filename);
	std::map<std::string, Animation> get_animations();
};

