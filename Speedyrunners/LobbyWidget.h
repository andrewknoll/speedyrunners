#pragma once
#include <vector>
#include "Settings.h"
#include "Background.h"
#include "UIElement.h"
class LobbyWidget : public sf::Drawable
{
	sf::RenderWindow& window;
	std::vector<Background> backgrounds;
	std::vector<std::shared_ptr<UIElement>> elements;

	int selectedCharacter = 0;

	enum class State {
		Initial,
		CharacterSelect
	};

	State state = State::Initial;

	bool isActive = false;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void addWidgetButton(const std::string& lobbyPath, const sf::Vector2f& pos, const float size, sf::RenderWindow& window, const Settings& settings, const int buttonIdx);

	sf::FloatRect getTexRectForCharacter(int idx);

	sf::FloatRect getNametagRectForCharacter(int idx);

	void addCharacterStuff(std::string lobbyPath, sf::RenderWindow& window, const sf::Vector2f& pos);

	void clear();


public:
	//LobbyWidget(const sf::Vector2f& pos, const Settings& settings);
	LobbyWidget(sf::RenderWindow& _window, const std::string lobbyPath, const sf::Vector2f& pos,
		bool active = false);

	LobbyWidget(sf::RenderWindow& _window, const Settings& settings, const std::string lobbyPath, const sf::Vector2f& pos, bool active);

	void handleClick(int idx);

	void update(sf::Event& event, const sf::Vector2f& mousePos);


};

