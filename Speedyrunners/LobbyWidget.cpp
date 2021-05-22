#include <iostream>
#include "LobbyWidget.h"
#include "UISprite.h"
#include "TextElement.h"
#include "utils.hpp"
#include "Resources.h"
#include "AudioPlayer.h"

void LobbyWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	int i = 0;
	for (auto bg : backgrounds) {
		//bg->draw(window);// 
		if (isActive || i!=1) target.draw(bg); // if inactive, skip the surrounding rect
		i++;
	}
	for (const auto& e : elements) {
		target.draw(*e);
		//e->draw(window);
	}

}


void LobbyWidget::addWidgetButton(const std::string& lobbyPath, const sf::Vector2f& pos, const float size, sf::RenderWindow& window, const Settings& settings, const int buttonIdx) {
	std::vector<std::string> texts = { "", "Runner", "Skin" };
	std::string text = texts[buttonIdx];
	std::string path = lobbyPath + "PlayerWidgetButton.png";
	// Define the textureRect (All in relative texture coordinates):
	float texWidth = 1.0 / 6.0;
	sf::Vector2f texIni(buttonIdx * texWidth, 0);
	sf::Vector2f texSize(texWidth, 0.5);
	std::vector<sf::FloatRect> rects{ sf::FloatRect(texIni, texSize), sf::FloatRect(texIni + sf::Vector2f(0,texSize.y), texSize) };
	elements.emplace_back(std::make_unique<UISprite>(path, settings, size, pos, true, rects));
	auto uisprite = elements.back();
	std::string mainTextFontPath = glb::CONTENT_PATH + "UI/Font/Souses.ttf";
	float textSize = size * 0.6;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, texts[buttonIdx], textSize, pos+sf::Vector2f(textSize,0), true));
	elements.back()->setClickableArea(uisprite->getClickableArea());
}

sf::FloatRect LobbyWidget::getTexRectForCharacter(int idx, int nRows) {
	int nCols = 8;
	float height = 0.96 / float(nRows);
	sf::Vector2f pos(0,0), size(1.0/float(nCols), height);
	if (idx == 0) { // Speedrunner
		// For some reason this row is smaller....
		float width = 245.0/2040.0;// medido a mano (1.02 - 1.0 / 16.0) / 8.0;
		pos.x = 2 * width;
		width = 225 / 2040.0;
		pos.y = 3.0 * height;
		size.x = width;
		size.y = height;
	}else if (idx == 1) { // Cosmo
		// For some reason this row is smaller....
		float width =  1.0 / 8.0;
		pos.x = 0; // thank you for being at 0
		pos.y = 2.0 * height;
		size.x = width;
		size.y = height;
	}
	else if (idx == 2) { // uniq
		// For some reason this row is smaller....
		float width = 1.0 / 8.0;
		pos.x = 1 * width;
		width = 225 / 2040.0;
		pos.y = 5.0 * height;
		size.x = width;
		size.y = height;
	}
	else if (idx == 3) { // falcon
		// For some reason this row is smaller....
		float width = 1.0 / 8.0;
		pos.x = 5.75 * width;
		pos.y = 3.0 * height;
		size.x = width;
		size.y = height;
	}
	else {
		std::cerr << "non implemented idx " << idx << " for lobby character portrait!!\n";
	}
	return sf::FloatRect(pos, size);
}


sf::FloatRect LobbyWidget::getNametagRectForCharacter(int idx) {
	float reduction = 0.95;
	if (idx == 2) idx = 1; // comrade is 2nd
	else if (idx == 1) idx = 2; // uniq 3rd
	else if (idx == 3) {
		idx = 7; // falcon 8th
		reduction = 0.90;
	}
	int nCols = 4, nRows = 11;
	sf::Vector2f pos(0, 0), size(1.0 / float(nCols), reduction / float(nRows));
	pos.x = (idx % nCols) * size.x;
	pos.y = (idx / nCols) * size.y;
	size.y *= 0.99;
	return sf::FloatRect(pos, size);
}

void LobbyWidget::addCharacterStuff(std::string lobbyPath, sf::RenderWindow& window, sf::Vector2f pos) {
	float h = pos.y;
	// portrait:
	// Get texture:
	std::string path = lobbyPath + "characterPortraits2.png";
	int rows = 5;
	if (selectedCharacter == glb::characterIndex::COSMONAUT)  pos.y += 40.0 / 1080.0;
	else if (selectedCharacter == glb::characterIndex::UNIC) {
		path = lobbyPath + "characterPortraits0.png";
		rows = 6;
	}
	else if (selectedCharacter == glb::characterIndex::FALCON) {
		path = lobbyPath + "characterPortraits1.png";
		rows = 7;
		pos.y -= 10.0 / 1080.0;
		h = pos.y;
	}
	// Set bg and rect:
	Background bg(path);
	bg.load(window, sf::FloatRect(pos.x, pos.y + 130.0 / 1080.0, 0.66 / 5.0, 1.0 / 6.0));
	bg.setTextureCoords(getTexRectForCharacter(selectedCharacter, rows));
	bg.fixProportions();
	backgrounds.emplace_back(bg);
	
	// Name tag:
	Background nameTags(lobbyPath + "NameTags_240x80.png");
	nameTags.load(window, sf::FloatRect(pos.x + 0.008, h + 60.0 / 1080.0, 0.66 / 5.0, 0.1));
	nameTags.setTextureCoords(getNametagRectForCharacter(selectedCharacter));
	nameTags.fixProportions();
	backgrounds.emplace_back(nameTags);

}

void LobbyWidget::clear() {
	auto b = backgrounds.front();
	backgrounds.clear(); 
	backgrounds.push_back(b); // Keep only the first background
	elements.clear();
}

LobbyWidget::LobbyWidget(sf::RenderWindow& _window, const Settings& settings, const std::string lobbyPath, const sf::Vector2f& pos, bool active, int idx)
	: window(_window), isActive(active), m_lobbyPath(lobbyPath), m_pos(pos), m_settings(settings), audio(Resources::getInstance().getAudioPlayer())
{
	// widget background:
	Background wbg(lobbyPath + "PlayerWidgetBackground.png");
	wbg.load(window, sf::FloatRect(pos.x, pos.y, 0.66 / 2.25, 0.9 / 2.25));
	wbg.setTextureCoords(sf::FloatRect(0.01, 0.02, 0.99, 0.98));
	backgrounds.emplace_back(wbg);
	//active = true;
	if (active) { // add surrounding rectangle


		//Outline:
		Background outline(lobbyPath + "CharacterSelectPortraitOutline.png");
		outline.load(window, sf::FloatRect(pos.x - 0.01, pos.y - 0.02, 0.66 / 2.1, 0.9 / 2.08));
		backgrounds.emplace_back(outline);
		addCharacterStuff(lobbyPath, window, pos);
		// Runner button:
		addWidgetButton(lobbyPath, pos+sf::Vector2f(0.18,0.1), 0.05, window, settings, 1);
	
	}
	else {
		std::string text = "ADD AI";
		if (idx == 1) text = "ADD PLAYER";
		std::string mainTextFontPath = glb::CONTENT_PATH + "UI/Font/Souses.ttf";
		float textSize = 0.04;
		elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, text, textSize, pos + sf::Vector2f(textSize, 0.2), true));
	}
}


void LobbyWidget::updateCharacter() {
	backgrounds.pop_back(); backgrounds.pop_back(); 
	addCharacterStuff(m_lobbyPath, window, m_pos);
}

/**
LobbyWidget::setCharacterSelect(sf::RenderWindow& _window, const Settings& settings, const std::string lobbyPath, const sf::Vector2f& pos, bool active)
	: window(_window), isActive(active)
{
	// widget background:
	backgrounds.emplace_back(lobbyPath + "PlayerWidgetBackground.png", window, sf::FloatRect(pos.x, pos.y, 0.66 / 2.25, 0.9 / 2.25));
	backgrounds.back().setTextureCoords(sf::FloatRect(0.01, 0.02, 0.99, 0.98));

	if (active) { // add surrounding rectangle

		backgrounds.emplace_back(lobbyPath + "CharacterSelectPortraitOutline.png", window, sf::FloatRect(pos.x - 0.01, pos.y - 0.02, 0.66 / 2.1, 0.9 / 2.08));
		// Runner:
		addWidgetButton(lobbyPath, pos + sf::Vector2f(0.15, 0.1), 0.05, window, settings, 1);
	}
}
*/



void LobbyWidget::handleClick(int idx) {
	if (isActive) {
		audio.play(AudioPlayer::Effect::MENU_TOGGLE);
		switch (idx) {
		case 0:
		{
			int sel = (int)selectedCharacter;
			selectedCharacter = glb::characterIndex((sel + 1) % 4);
			updateCharacter();
			break;
		}
		case 1:
		{
			elements.clear();
			state = State::CharacterSelect;
			//setCharacterSelect();
			break;
		}
		default:
			std::cout << "clicked element " << idx << "\n";
		}
	}
	else {
		audio.play(AudioPlayer::Effect::MENU_PLAYER_ENTERLOBBY);
		isActive = true;
		elements.pop_back();
		//Outline:
		Background out(m_lobbyPath + "CharacterSelectPortraitOutline.png");
		out.load(window, sf::FloatRect(m_pos.x - 0.01, m_pos.y - 0.02, 0.66 / 2.1, 0.9 / 2.08));
		backgrounds.emplace_back(out);
		addCharacterStuff(m_lobbyPath, window, m_pos);
		// Runner button:
		addWidgetButton(m_lobbyPath, m_pos + sf::Vector2f(0.18, 0.1), 0.05, window, m_settings, 1);
	}
}

void LobbyWidget::update(sf::Event &event, const sf::Vector2f& mousePos)
{
#ifdef DEBUG_MOUSE_POS
	std::cout << ""
#endif
	if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed) { // Click
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->mouseInside(window) && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
				handleClick(i);
				break;
			}
			//else {
				//isActive = false;
			//}
		}
	}
}

glb::characterIndex LobbyWidget::getSelectedCharacter() const
{
	return selectedCharacter;
}

bool LobbyWidget::activated() const {
	return isActive;
}

