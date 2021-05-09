#include <iostream>
#include "LobbyWidget.h"
#include "UISprite.h"
#include "TextElement.h"

void LobbyWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto bg : backgrounds) {
		//bg->draw(window);// 
		target.draw(bg);
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
	std::string mainTextFontPath = glb::CONTENT_PATH + "UI/Font/Souses.ttf";
	float textSize = size * 0.6;
	elements.emplace_back(std::make_unique<TextElement>(settings, mainTextFontPath, texts[buttonIdx], textSize, pos+sf::Vector2f(textSize,0), false));
}


LobbyWidget::LobbyWidget(sf::RenderWindow& _window, const Settings& settings, const std::string lobbyPath, const sf::Vector2f& pos, bool active)
	: window(_window), isActive(active)
{
	// widget background:
	backgrounds.emplace_back(lobbyPath + "PlayerWidgetBackground.png", window, sf::FloatRect(pos.x, pos.y, 0.66 / 2.25, 0.9 / 2.25));
	backgrounds.back().setTextureCoords(sf::FloatRect(0.01, 0.02, 0.99, 0.98));

	if (active) { // add surrounding rectangle

		backgrounds.emplace_back(lobbyPath + "CharacterSelectPortraitOutline.png", window, sf::FloatRect(pos.x-0.01, pos.y-0.02, 0.66 / 2.1, 0.9 / 2.08));
		// Runner:
		addWidgetButton(lobbyPath, pos+sf::Vector2f(0.15,0.1), 0.05, window, settings, 1);
	}
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
	switch (idx) {
	case 0:
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
		}
	}
}
