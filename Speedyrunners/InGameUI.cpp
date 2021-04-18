#include "InGameUI.h"
#include "UISprite.h"
#include "Game.h"

void InGameUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& e : sprites) {
		target.draw(*e);
		//e->draw(target);
	}
}


InGameUI::InGameUI() : window(nullptr)
{
}

InGameUI::InGameUI(sf::RenderWindow& _window) : window(&_window)
{
}

void InGameUI::setWindow(sf::RenderWindow& _window)
{
	window = &_window;
}

void InGameUI::setCharacters(std::vector<CharPtr> characters) {
	// Position:
	sf::Vector2f pos(0.1, 0.02); // initial
	pos = utils::relativeToGlobal(pos, *window);
	sf::Vector2f headDisplacement(0.01,0.01);
	headDisplacement = utils::relativeToGlobal(headDisplacement, *window);
	float scale = 0.1;
	// For each character:
	for (auto c : characters) {
		// TODO: COLOCAR EN SU SITIO CON SU TAMAÑO ETC
		// blue rectangle thing:
		std::string passportBg = glb::PASSPORT_PATH + "PlayerPassport_Background.png";
		sprites.emplace_back(std::make_unique<UISprite>(passportBg));
		sprites.back()->setPosition(pos);
		// head:
		sprites.emplace_back(std::make_unique<UISprite>(c->getUIIconPath()));
		sprites.back()->setHead(pos, scale, *window);
	}
}



