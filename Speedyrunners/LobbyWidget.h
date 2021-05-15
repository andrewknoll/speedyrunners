#pragma once
#include <vector>
#include "Settings.h"
#include "Background.h"
#include "UIElement.h"

class AudioPlayer;

class LobbyWidget : public sf::Drawable
{
	sf::RenderWindow& window;
	std::vector<Background> backgrounds;
	std::vector<std::shared_ptr<UIElement>> elements;

	glb::characterIndex selectedCharacter = glb::characterIndex::SPEEDRUNNER;

	enum class State {
		Initial,
		CharacterSelect
	};
	std::string m_lobbyPath;
	sf::Vector2f m_pos;
	const Settings& m_settings;
	State state = State::Initial;
	bool isActive = false;
	AudioPlayer& audio;


	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void addWidgetButton(const std::string& lobbyPath, const sf::Vector2f& pos, const float size, sf::RenderWindow& window, const Settings& settings, const int buttonIdx);
	sf::FloatRect getTexRectForCharacter(int idx, int nRows);
	sf::FloatRect getNametagRectForCharacter(int idx);
	void addCharacterStuff(std::string lobbyPath, sf::RenderWindow& window, sf::Vector2f pos);
	void clear();

public:
	//LobbyWidget(const sf::Vector2f& pos, const Settings& settings);
	LobbyWidget(sf::RenderWindow& _window, const std::string lobbyPath, const sf::Vector2f& pos,
		bool active = false);

	LobbyWidget(sf::RenderWindow& _window, const Settings& settings, const std::string lobbyPath, const sf::Vector2f& pos, bool active, int idx);

	void updateCharacter();

	void handleClick(int idx);

	void update(sf::Event& event, const sf::Vector2f& mousePos);


	glb::characterIndex getSelectedCharacter() const;

	bool activated() const; // true if the widget has selected a character

};

