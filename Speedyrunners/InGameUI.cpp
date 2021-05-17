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
	for (const auto& pts : roundPoints) // points
		for (const auto& p : pts)
			target.draw(p);
	for (const auto& bs : boostSprites) // boost bars
		for (const auto& b : bs)
			target.draw(b);
	for (const auto& is : items) // boost bars
		for (const auto& i : is)
			target.draw(i);
	target.setView(v);
}


InGameUI::InGameUI() : window(nullptr), pointsTexture(Resources::getInstance().getMiscTexture(glb::SCORE_POINT_TEX))
{
}

InGameUI::InGameUI(sf::RenderWindow& _window) : window(&_window), pointsTexture(Resources::getInstance().getMiscTexture(glb::SCORE_POINT_TEX))
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

void InGameUI::addBoostBar(sf::Vector2f pos, float widgetWidth) {
	// Back rectangle
	auto &b = boostSprites.emplace_back();
	pos.y += widgetWidth * 0.28f;
	const auto& t = Resources::getInstance().getMiscTexture(glb::BOOST_BAR_TEX);
	auto &back = b.emplace_back(t);
	auto size = t.getSize();
	size.y /= 3;
	back.setTextureRect(sf::IntRect(0,0,size.x, size.y));
	utils::setWidth(back, widgetWidth);
	back.setPosition(pos);
	// bar:
	auto& bar = b.emplace_back(t);
	bar.setTextureRect(sf::IntRect(0, size.y, size.x, size.y));
	//utils::scaleToFullScreenRatio(bar, *window, 0.1);
	utils::setWidth(bar, widgetWidth);
	bar.setPosition(pos);
	boostTexSize = size;
}


void InGameUI::addItems(sf::Vector2f pos, float widgetWidth) {
	pos.x -= widgetWidth / 2.0f; // To the left
	// Item Background
	auto& b = items.emplace_back();
	const auto& t = Resources::getInstance().getMiscTexture(glb::POWER_UP_HUD_BG_TEX);
	auto& back = b.emplace_back(t);
	auto size = t.getSize();
	size.y /= 2; size.x /= 4;
	bgItemTexRect = sf::IntRect(0, 0, size.x, size.y);
	back.setTextureRect(bgItemTexRect);
	utils::setWidth(back, widgetWidth/2.0);
	back.setPosition(pos);
	// Item:
	const auto& itemT = Resources::getInstance().getMiscTexture(glb::POWER_UP_HUD_TEX);
	auto& i = b.emplace_back(itemT);
	size = itemT.getSize();
	size.x /= 18; // 18 slots in texture, horizontally
	i.setTextureRect(sf::IntRect(6*size.x, 0, size.x, size.y));
	//utils::scaleToFullScreenRatio(bar, *window, 0.1);
	float w = widgetWidth / 2.0;
	utils::setWidth(i, 0.8 * w);
	i.setPosition(pos+sf::Vector2f(w*0.1f, w * 0.1f));
	itemTexSize = size;
}

void InGameUI::setCharacters(std::vector<CharPtr> characters) {
	chars = characters;
	roundPoints.clear();
	boostSprites.clear();
	items.clear();

	// Position:
	sf::Vector2f pos(0.05, 0.005); // initial
	pos = utils::relativeToGlobal(pos, *window);
	sf::Vector2f headDisplacement(-0.03,0.36);
	headDisplacement = utils::relativeToGlobal(headDisplacement, *window);
	int nSlots = std::max((int)characters.size(), 3); // if less than 3, dont make them bigger
	float scale = 0.15 * 2.0 / nSlots, headScale = scale*0.35; // vertical size relative to window
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

		addBoostBar(pos, widgetWidth*1.5f);

		addItems(pos, widgetWidth);
		
		//pos.x += scale * 3.5 * window->getSize().x;
		pos.x += 3.4 * scale * window->getSize().x;
	}
	updatePoints();
	std::cout << sprites.size() << " sprites en UI\n";
}

void InGameUI::updateBoostBar(int charidx, float proportion) {
	boostSprites[charidx][1].setTextureRect(sf::IntRect(0, boostTexSize.y, proportion * boostTexSize.x, boostTexSize.y));
}


void InGameUI::updateItem(int charidx, glb::item item) {
	int itemIdx = glb::itemToTexIndex[item];
	if (item == glb::item::NONE) // is none, dont show background
		items[charidx][0].setTextureRect(sf::IntRect(0,0,0,0));
	else // Show background:
		items[charidx][0].setTextureRect(bgItemTexRect);
	// Update the item icon:
	items[charidx][1].setTextureRect(sf::IntRect(itemIdx * itemTexSize.x, 0, itemTexSize.x, itemTexSize.y));
}


void InGameUI::update() {
	for (int i = 0; i < chars.size(); i++) {
		float boost = chars[i]->getRemainingBoost01();
		updateBoostBar(i, boost);
		updateItem(i, chars[i]->getCurrentItem());
	}
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
			roundPoints[i][j].setTextureRect(texRectFor(j + 1));
		}
	}
}
