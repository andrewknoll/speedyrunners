#include "InGameUI.h"
#include "UISprite.h"
#include "Character.h"

void InGameUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& e : elements) {
		target.draw(*e);
		//e->draw(target);
	}
}

InGameUI::InGameUI(std::vector<CharPtr> characters)
{
	// For each character:
	for (auto c : characters) {
		// TODO: COLOCAR EN SU SITIO CON SU TAMAÑO ETC
		// blue rectangle thing:
		std::string passportBg = glb::PASSPORT_PATH + "PlayerPassport_Background.png"; 
		elements.emplace_back(std::make_unique<UISprite>(passportBg));

		// head:
		elements.emplace_back(std::make_unique<UISprite>(c->getUIIconPath()));
	}
	
}



