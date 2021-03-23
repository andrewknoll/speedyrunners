#include "Spritesheet.h"

bool Spritesheet::remove_commas_or_spaces(std::string& matched) {
	std::string substring;
	std::string without_spaces = matched;
	if (matched.empty()) return false;
	//REMOVE INITIAL SPACES
	int i = 0;
	while (matched[i] == ' ' && i < matched.size()) {
		matched.erase(i, 1);
	}
	if (matched.empty()) return false;
	//REMOVE SPACES AT THE END
	i = matched.size() - 1;
	while (matched[i] == ' ') {
		matched.erase(i, 1);
	}
	if (matched.empty()) return false;
	if (matched[0] == '"' && matched[matched.size() - 1] == '"') {
		substring = matched.substr(1, matched.size()-2); //Remove first and last
		matched = substring;
		return true;
	}
	else {
		//REMOVE ALL SPACES
		i = 0;
		while (i < matched.size()) {
			if (matched[i] == ' ') matched.erase(i, 1);
			else i++;
		}
		return false;
	}
}

int Spritesheet::process_token(std::string matched) {
	static int last_frame_index;
	static std::string last_animation_name;
	static int coord_cache[4];
	bool has_commas = remove_commas_or_spaces(matched);
	if (matched.empty()) return 0;
	else if (matched == "{") {
		if (flags[MetaScope]) flags[MetaSubscope] = true;
		else if (!flags[GlobalScope]) flags[GlobalScope] = true;
		else if (!flags[FramesToken]) return MissingFrames;
		else if (!flags[FramesScope]) return MissingBracket;
		else if (!flags[SingleFrameScope]) flags[SingleFrameScope] = true;
		else if (flags[FilenameToken] && !flags[FilenameRead]) return UnexpectedToken;
		else if (flags[PositionToken] && !flags[PositionScope]) {
			if (!flags[Position_X] && !flags[Position_Y] && !flags[Position_W] && !flags[Position_H]) flags[PositionScope] = true;
			else flags[UnusedSubscope] = true;
		}
		else return UnexpectedToken;
	}
	else if (matched == "}") {
		if (flags[MetaSubscope]) flags[MetaSubscope] = false;
		else if (flags[MetaScope]) flags[MetaScope] = false;
		else if (flags[UnusedSubscope]) flags[UnusedSubscope] = false;
		else if (flags[PositionToken] && flags[PositionScope]) flags[PositionScope] = false;
		else if (flags[FilenameToken] && !flags[FilenameRead]) return UnexpectedToken;
		else if (flags[SingleFrameScope]) {
			if (flags[FilenameRead] && flags[Position_H]) { //No errors occurred
				sf::IntRect new_rect = sf::IntRect(coord_cache[0], coord_cache[1], coord_cache[2], coord_cache[3]);
				if (animations.count(last_animation_name) == 0) {	//Create Animation if this is the first frame added
					AnimationPtr new_animation = std::make_shared<Animation>();
					animations.insert(std::pair<std::string, AnimationPtr>(last_animation_name, new_animation));
					animations[last_animation_name]->set_spritesheet(texture);
				}
				if (!animations[last_animation_name]->insert(last_frame_index, new_rect)) return DuplicatedFrame;
			}
			flags[SingleFrameScope] = false;
			flags[FilenameToken] = false;
			flags[FilenameRead] = false;
			flags[PositionToken] = false;
			flags[Position_X] = false;
			flags[Position_Y] = false;
			flags[Position_W] = false;
			flags[Position_H] = false;
		}
		else if (flags[GlobalScope]) {
			flags[GlobalScope] = false;
			return Finished;
		}
		else return MissingCurlyBrace;
	}
	else if (matched == "frames" && !flags[MetaScope]) {
		if (!flags[GlobalScope]) return MissingCurlyBrace;
		else if (flags[FramesToken]) return UnexpectedToken;
		else flags[FramesToken] = true;
	}
	else if (matched == "[" && !flags[MetaScope]) {
		if (!flags[GlobalScope]) return MissingCurlyBrace;
		else if (!flags[FramesToken]) return MissingFrames;
		else if (flags[UnusedSubscope]) return 0;
		else if (!flags[FramesScope]) flags[FramesScope] = true;
		else if (flags[FilenameToken] && !flags[FilenameRead]) return UnexpectedToken;
		else return UnexpectedToken;
	}
	else if (matched == "]" && !flags[MetaScope]) {
		if (!flags[GlobalScope]) return MissingCurlyBrace;
		else if (!flags[FramesToken]) return MissingFrames;
		else if (flags[UnusedSubscope]) return 0;
		else if (flags[FilenameToken] && !flags[FilenameRead]) return UnexpectedToken;
		else if (flags[PositionToken] || flags[SingleFrameScope]) return UnexpectedToken;
		else if (flags[FramesScope]) flags[FramesScope] = false;
		else return MissingBracket;
	}
	else if (matched == "meta" && !flags[MetaScope]) {
		if (!flags[GlobalScope]) return MissingCurlyBrace;
		else if (flags[UnusedSubscope]) return 0;
		else if (flags[FilenameToken] && !flags[FilenameRead]) return UnexpectedToken;
		else if (flags[FramesScope] || flags[PositionScope]) return UnexpectedToken;
		else flags[MetaScope] = true;
	}
	else if (matched == "filename" && !flags[MetaScope]) {
		if (!flags[GlobalScope]) return MissingCurlyBrace;
		else if (!flags[FramesToken]) return MissingFrames;
		else if (!flags[FramesScope]) return MissingBracket;
		else if (flags[UnusedSubscope]) return 0;
		else if (!flags[SingleFrameScope] || flags[PositionScope]) return UnexpectedToken;
		else if (!flags[FilenameToken]) flags[FilenameToken] = true;
		else return DuplicatedFilenameToken;
	}
	else if (matched == "frame" && !flags[MetaScope]) {
		if (!flags[GlobalScope]) return MissingCurlyBrace;
		else if (!flags[FramesToken]) return MissingFrames;
		else if (!flags[FramesScope]) return MissingBracket;
		else if (flags[UnusedSubscope]) return 0;
		else if (flags[FilenameToken] && !flags[FilenameRead]) return UnexpectedToken;
		else if (!flags[SingleFrameScope] || flags[PositionScope]) return UnexpectedToken;
		else if (!flags[PositionToken]) flags[PositionToken] = true;
		else return DuplicatedPosition;
	}
	else if (flags[FilenameToken] && !flags[FilenameRead] && !flags[UnusedSubscope]) {
		int i = matched.size() - 1;
		while (i > 0 && matched[i] != '.') i--; //Ignore extension
		int frame_index = std::stoi(matched.substr(i - 4, 4)); //Get Frame Number
		std::string animation_name = matched.substr(0, i - 4);
		last_frame_index = frame_index;
		last_animation_name = animation_name;
		flags[FilenameRead] = true;
	}
	else if (flags[PositionToken] && flags[PositionScope] && !flags[UnusedSubscope]) {
		if (has_commas) {
			if (matched == "x") {
				if (flags[Position_X]) return InvalidPosition;
				else flags[Position_X] = true;
			}
			else if (matched == "y") {
				if (!flags[Position_X] || flags[Position_Y]) return InvalidPosition;
				else flags[Position_Y] = true;
			}
			else if (matched == "w") {
				if (!flags[Position_Y] || flags[Position_W]) return InvalidPosition;
				else flags[Position_W] = true;
			}
			else if (matched == "h") {
				if (!flags[Position_W] || flags[Position_H]) return InvalidPosition;
				else flags[Position_H] = true;
			}
		}
		else {
			if (flags[Position_X] && !flags[Position_Y]) coord_cache[0] = std::stoi(matched);
			else if (flags[Position_Y] && !flags[Position_W]) coord_cache[1] = std::stoi(matched);
			else if (flags[Position_W] && !flags[Position_H]) coord_cache[2] = std::stoi(matched);
			else if (flags[Position_H]) coord_cache[3] = std::stoi(matched);
		}

	}
	return 0;
}

int Spritesheet::parse_spritesheet(std::string image_filename, std::string data_filename) {
	std::ifstream data(data_filename);
	int return_code = 0;
	if (data.is_open()) {
		texture = std::make_shared<sf::Texture>();
		if (!texture->loadFromFile(image_filename)) {
			std::cerr << "Image file " << image_filename << " is missing!" << std::endl;
			return ImageFileMissing;
		}
	}
	else {
		std::cerr << "Data file " << data_filename << " is missing!" << std::endl;
		return DataFileMissing;
	}
	std::string matched = "";
	char c;
	while (data.get(c) && return_code>=0) {
		if ((!flags[UnusedSubscope] && !flags[MetaScope] && !flags[MetaSubscope]) || c == '{' || c == '}') {
			if (c == ':' || c == ',') {
				return_code = process_token(matched);
				matched = "";
			}
			else if (c == '[' || c == ']' || c == '{' || c == '}') {
				if (!matched.empty()) return_code = process_token(matched);
				matched = c;
				return_code = process_token(matched);
				matched = "";
			}
			else if (isalnum(c) || c == ' ' || c == '"' || c == '.' || c == '-' || c == '_') {
				matched += c;
			}
			else if (c != '\n' && c != '\t') {
				std::cerr << "Unrecognized character " << c << std::endl;
				return UnrecognizedCharacter;
			}
		}
	}
	return return_code;
}

std::map<std::string, AnimationPtr> Spritesheet::get_animations() {
	return animations;
}