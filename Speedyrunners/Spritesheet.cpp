#include "Spritesheet.h"

#define FINISHED 1
#define DATA_FILE_MISSING -1
#define IMAGE_FILE_MISSING -2
#define UNRECOGNIZED_CHARACTER -3
#define MISSING_CURLY_BRACE -4
#define MISSING_BRACKET -5
#define MISSING_FRAMES -6
#define UNEXPECTED_TOKEN -7
#define DUPLICATED_FILENAME_TOKEN -8
#define DUPLICATED_POSITION -9
#define DUPLICATED_FRAME -10
#define INVALID_POSITION -11

#define global_scope_flag flags[0]
#define frames_token_flag flags[1]
#define frames_scope_flag flags[2]
#define single_frame_scope_flag flags[3]
#define filename_token_flag flags[4]
#define filename_read_flag flags[5]
#define position_token_flag flags[6]
#define position_scope_flag flags[7]
#define unused_subscope_flag flags[8]
#define position_x_token_flag flags[9]
#define position_y_token_flag flags[10]
#define position_w_token_flag flags[11]
#define position_h_token_flag flags[12]
#define meta_scope_flag flags[13]
#define meta_subscope_flag flags[14]

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
		if (meta_scope_flag) meta_subscope_flag = true;
		else if (!global_scope_flag) global_scope_flag = true;
		else if (!frames_token_flag) return MISSING_FRAMES;
		else if (!frames_scope_flag) return MISSING_BRACKET;
		else if (!single_frame_scope_flag) single_frame_scope_flag = true;
		else if (filename_token_flag && !filename_read_flag) return UNEXPECTED_TOKEN;
		else if (position_token_flag && !position_scope_flag) {
			if (!position_x_token_flag && !position_y_token_flag && !position_w_token_flag && !position_h_token_flag) position_scope_flag = true;
			else unused_subscope_flag = true;
		}
		else return UNEXPECTED_TOKEN;
	}
	else if (matched == "}") {
		if (meta_subscope_flag) meta_subscope_flag = false;
		else if (meta_scope_flag) meta_scope_flag = false;
		else if (unused_subscope_flag) unused_subscope_flag = false;
		else if (position_token_flag && position_scope_flag) position_scope_flag = false;
		else if (filename_token_flag && !filename_read_flag) return UNEXPECTED_TOKEN;
		else if (single_frame_scope_flag) {
			if (filename_read_flag && position_h_token_flag) { //No errors occurred
				sf::IntRect new_rect = sf::IntRect(coord_cache[0], coord_cache[1], coord_cache[2], coord_cache[3]);
				if (animations.count(last_animation_name) == 0) {	//Create Animation if this is the first frame added
					Animation new_animation;
					animations.insert(std::pair<std::string, Animation>(last_animation_name, new_animation));
					animations[last_animation_name].set_spritesheet(texture);
				}
				if (!animations[last_animation_name].insert(last_frame_index, new_rect)) return DUPLICATED_FRAME;
			}
			single_frame_scope_flag = false;
			filename_token_flag = false;
			filename_read_flag = false;
			position_token_flag = false;
			position_x_token_flag = false;
			position_y_token_flag = false;
			position_w_token_flag = false;
			position_h_token_flag = false;
		}
		else if (global_scope_flag) {
			global_scope_flag = false;
			return FINISHED;
		}
		else return MISSING_CURLY_BRACE;
	}
	else if (matched == "frames" && !meta_scope_flag) {
		if (!global_scope_flag) return MISSING_CURLY_BRACE;
		else if (frames_token_flag) return UNEXPECTED_TOKEN;
		else frames_token_flag = true;
	}
	else if (matched == "[" && !meta_scope_flag) {
		if (!global_scope_flag) return MISSING_CURLY_BRACE;
		else if (!frames_token_flag) return MISSING_FRAMES;
		else if (unused_subscope_flag) return 0;
		else if (!frames_scope_flag) frames_scope_flag = true;
		else if (filename_token_flag && !filename_read_flag) return UNEXPECTED_TOKEN;
		else return UNEXPECTED_TOKEN;
	}
	else if (matched == "]" && !meta_scope_flag) {
		if (!global_scope_flag) return MISSING_CURLY_BRACE;
		else if (!frames_token_flag) return MISSING_FRAMES;
		else if (unused_subscope_flag) return 0;
		else if (filename_token_flag && !filename_read_flag) return UNEXPECTED_TOKEN;
		else if (position_token_flag || single_frame_scope_flag) return UNEXPECTED_TOKEN;
		else if (frames_scope_flag) frames_scope_flag = false;
		else return MISSING_BRACKET;
	}
	else if (matched == "meta" && !meta_scope_flag) {
		if (!global_scope_flag) return MISSING_CURLY_BRACE;
		else if (unused_subscope_flag) return 0;
		else if (filename_token_flag && !filename_read_flag) return UNEXPECTED_TOKEN;
		else if (frames_scope_flag || position_scope_flag) return UNEXPECTED_TOKEN;
		else meta_scope_flag = true;
	}
	else if (matched == "filename" && !meta_scope_flag) {
		if (!global_scope_flag) return MISSING_CURLY_BRACE;
		else if (!frames_token_flag) return MISSING_FRAMES;
		else if (!frames_scope_flag) return MISSING_BRACKET;
		else if (unused_subscope_flag) return 0;
		else if (!single_frame_scope_flag || position_scope_flag) return UNEXPECTED_TOKEN;
		else if (!filename_token_flag) filename_token_flag = true;
		else return DUPLICATED_FILENAME_TOKEN;
	}
	else if (matched == "frame" && !meta_scope_flag) {
		if (!global_scope_flag) return MISSING_CURLY_BRACE;
		else if (!frames_token_flag) return MISSING_FRAMES;
		else if (!frames_scope_flag) return MISSING_BRACKET;
		else if (unused_subscope_flag) return 0;
		else if (filename_token_flag && !filename_read_flag) return UNEXPECTED_TOKEN;
		else if (!single_frame_scope_flag || position_scope_flag) return UNEXPECTED_TOKEN;
		else if (!position_token_flag) position_token_flag = true;
		else return DUPLICATED_POSITION;
	}
	else if (filename_token_flag && !filename_read_flag && !unused_subscope_flag) {
		int i = matched.size() - 1;
		while (i > 0 && matched[i] != '.') i--; //Ignore extension
		int frame_index = std::stoi(matched.substr(i - 4, 4)); //Get Frame Number
		std::string animation_name = matched.substr(0, i - 4);
		last_frame_index = frame_index;
		last_animation_name = animation_name;
		filename_read_flag = true;
	}
	else if (position_token_flag && position_scope_flag && !unused_subscope_flag) {
		if (has_commas) {
			if (matched == "x") {
				if (position_x_token_flag) return INVALID_POSITION;
				else position_x_token_flag = true;
			}
			else if (matched == "y") {
				if (!position_x_token_flag || position_y_token_flag) return INVALID_POSITION;
				else position_y_token_flag = true;
			}
			else if (matched == "w") {
				if (!position_y_token_flag || position_w_token_flag) return INVALID_POSITION;
				else position_w_token_flag = true;
			}
			else if (matched == "h") {
				if (!position_w_token_flag || position_h_token_flag) return INVALID_POSITION;
				else position_h_token_flag = true;
			}
		}
		else {
			if (position_x_token_flag && !position_y_token_flag) coord_cache[0] = std::stoi(matched);
			else if (position_y_token_flag && !position_w_token_flag) coord_cache[1] = std::stoi(matched);
			else if (position_w_token_flag && !position_h_token_flag) coord_cache[2] = std::stoi(matched);
			else if (position_h_token_flag) coord_cache[3] = std::stoi(matched);
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
			return IMAGE_FILE_MISSING;
		}
	}
	else {
		std::cerr << "Data file " << data_filename << " is missing!" << std::endl;
		return DATA_FILE_MISSING;
	}
	std::string matched = "";
	char c;
	while (data.get(c) && return_code>=0) {
		if ((!unused_subscope_flag && !meta_scope_flag && !meta_subscope_flag) || c == '{' || c == '}') {
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
				return UNRECOGNIZED_CHARACTER;
			}
		}
	}
	return return_code;
}

std::map<std::string, Animation> Spritesheet::get_animations() {
	return animations;
}