#include "InGameUI.h"
#include "UISprite.h"
#include "Game.h"
#include "Resources.h"


void InGameUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& e : sprites) {
		target.draw(*e);
		//e->draw(target);
	}
	auto v = target.getView(); target.setView(target.getDefaultView());
	for (const auto& pts : roundPoints)
		for (const auto& p : pts)
			target.draw(p);
	target.setView(v);
}


InGameUI::InGameUI() : window(nullptr), pointsTexture(Resources::getInstance().getMiscTexture(1))
{
}

InGameUI::InGameUI(sf::RenderWindow& _window) : window(&_window), pointsTexture(Resources::getInstance().getMiscTexture(1))
{
}

void InGameUI::setWindow(sf::RenderWindow& _window)
{
	window = &_window;
}

void InGameUI::addPoints(sf::Vector2f pos, float widgetWidth) {
	auto& points = roundPoints.back(); // The vector
	pos.x += widgetWidth / 3.0; pos.y += widgetWidth / 6.0;
	for (int i = 0; i < 3; i++) { // 3 rounds
		points.emplace_back(pointsTexture);
		points.back().setPosition(pos);
		texSize = pointsTexture.getSize();
		int nPoints = 7;
		texSize.x = texSize.x / nPoints;
		points.back().setTextureRect(texRectFor(7));

		//float scale = window->getSize().x / points.back().getGlobalBounds().width;
		//float scale = 1000;
		utils::scaleToFullScreenRatio(points.back(), *window, 0.03);
		//points.back().setScale(scale, scale);
		pos.x += widgetWidth / 3.0;
	}
}

void InGameUI::setCharacters(std::vector<CharPtr> characters) {
	chars = characters;
	roundPoints.clear();

	// Position:
	sf::Vector2f pos(0.05, 0.005); // initial
	pos = utils::relativeToGlobal(pos, *window);
	sf::Vector2f headDisplacement(-0.03,0.36);
	headDisplacement = utils::relativeToGlobal(headDisplacement, *window);
	float scale = 0.15 * 2.0 / characters.size(), headScale = scale*0.35; // vertical size relative to window
	headDisplacement *= scale;
	std::string path = glb::CONTENT_PATH + "UI/MultiplayerHUD/";
	std::cout << "hay " << characters.size() << " characters\n";
	// Reset:
	sprites.clear();
	// For each character:
	for (auto c : characters) {
		// blue rectangle thing:
		sprites.emplace_back(std::make_unique<UISprite>(path + "Messages.png"));// , settings, scale, pos, false));
		sprites.back()->setPosition(pos);
		sprites.back()->setRelativeScale(scale, *window);
		sprites.back()->setIgnoreCamera(true);
		float widgetWidth = scale * window->getSize().x;

		// head:
		sprites.emplace_back(std::make_unique<UISprite>(c->getUIIconPath()));
		sprites.back()->setHead(pos+headDisplacement, headScale, *window);
		sprites.back()->setIgnoreCamera(true);
		// rounds:
		roundPoints.emplace_back(); // New vector for this character
		addPoints(pos, widgetWidth);
		
		//pos.x += scale * 3.5 * window->getSize().x;
		pos.x += 3.4 * scale * window->getSize().x;
	}
	updatePoints();
	std::cout << sprites.size() << " sprites en UI\n";
}

sf::IntRect InGameUI::texRectFor(int point) {
	return sf::IntRect((point - 1) * texSize.x, 0, texSize.x, texSize.y);
}

void InGameUI::updatePoints() {
	auto texSize = pointsTexture.getSize();
	int N_POINTS = 7;
	sf::IntRect rect(0,0,texSize.x / N_POINTS, texSize.y);
	for (int i = 0; i < chars.size(); i++) {
		int points = chars[i]->getScore();
		for (int j = 0; j < points; j++) {
			roundPoints[i][j].setTextureRect(texRectFor(j+1));
		}
		/*for (auto& point : charPoints) {
			point.setTextureRect(rect);
		}*/
	}
}
