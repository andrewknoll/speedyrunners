#include "InGameUI.h"
#include "UISprite.h"
#include "Game.h"

// TODO: USAR Content\UI\MultiplayerHUD !!!!!!!!!!!! es donde esta todo!!!!!!! no en UI/ingame........


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
	sf::Vector2f pos(0.05, 0.005); // initial
	pos = utils::relativeToGlobal(pos, *window);
	sf::Vector2f headDisplacement(-0.02,0.46);
	headDisplacement = utils::relativeToGlobal(headDisplacement, *window);
	float scale = 0.15 * 2.0 / characters.size(), headScale = scale*0.4; // vertical size relative to window
	headDisplacement *= scale;
	std::string passportBg = glb::PASSPORT_PATH + "PlayerPassport_Background.png";
	std::cout << "hay " << characters.size() << " characters\n";
	// Reset:
	sprites.clear();
	// For each character:
	for (auto c : characters) {
		// TODO: COLOCAR EN SU SITIO CON SU TAMAÑO ETC
		// blue rectangle thing:
		sprites.emplace_back(std::make_unique<UISprite>(passportBg));
		sprites.back()->setPosition(pos);
		sprites.back()->setRelativeScale(scale, *window);
		sprites.back()->setIgnoreCamera(true);
		// head:
		sprites.emplace_back(std::make_unique<UISprite>(c->getUIIconPath()));
		sprites.back()->setHead(pos+headDisplacement, headScale, *window);
		sprites.back()->setIgnoreCamera(true);
		//pos.x += scale * 3.5 * window->getSize().x;
		pos.x += 3.4 * scale * window->getSize().x;
	}
	std::cout << sprites.size() << " sprites en UI\n";
}



