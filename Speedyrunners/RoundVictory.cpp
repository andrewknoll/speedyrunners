#include "RoundVictory.h"
#include "Resources.h"
#include "utils.hpp"

RoundVictory::RoundVictory(const int characterIdx, const int characterVariant, const int characterScore) :
	currentSecond(3),
	t(sf::seconds(0.5)),
	audioPlayer(Resources::getInstance().getAudioPlayer()),
	characterScore(characterScore) 
{
	// Fondo:
	bg.loadFromFile(bgPath);
	bgSprite.setTexture(bg);
	bgSprite.setPosition(0, height / 2.0f);

	sf::IntRect rect;
	rect.left = 0;
	rect.width = bg.getSize().x;
	rect.height = bg.getSize().y / 2;
	rect.top = 0;
	bgSprite.setTextureRect(rect);

	
	//bgSprite.setScale(relation, relation);
	auto& spritesheet = Resources::getInstance().getVictorySpriteSheet(characterIdx, characterVariant);
	characterVictoryPose = spritesheet.get_animations()[0];
	mySprite = characterVictoryPose->get_first_frame();
}

void RoundVictory::setWindow(const sf::RenderWindow & _window) {
	width = _window.getDefaultView().getSize().x;
	height = _window.getDefaultView().getSize().y;

	utils::scaleToFullScreenRatio(bgSprite, _window, 0.4);
	utils::scaleToFullScreenRatio(mySprite, _window, 0.4);

	mySprite.setPosition(width * 0.2f, height * 0.882f);

	addScoreStuff(_window, characterScore);
}

void RoundVictory::setRectForPoint(sf::Sprite& s, int points) {
	auto texSize = Resources::getInstance().getMiscTexture(glb::SCORE_POINT_TEX).getSize();
	texSize.x /= 7; // 7 sprites horizontally
	s.setTextureRect(sf::IntRect((points - 1) * texSize.x, 0, texSize.x, texSize.y));
}

void RoundVictory::addScoreStuff(const sf::RenderWindow& _window, int score) {
	sprites.clear();
	auto size = _window.getDefaultView().getSize();
	sf::Vector2f pos(size.x * 0.45, size.y* 0.6);
	if (score == 3) { // Winner
		sprites.emplace_back(Resources::getInstance().getMiscTexture(glb::WINNER_LABEL_TEX));
		sprites.back().setPosition(pos);
		utils::scaleToFullScreenRatio(sprites.back(), _window, 0.2);
	}
	else { // Display points
		for (int i = 0; i < score; i++) {
			sprites.emplace_back(Resources::getInstance().getMiscTexture(glb::SCORE_POINT_TEX));
			auto& s = sprites.back();
			setRectForPoint(s, i + 1);
			utils::scaleToFullScreenRatio(s, _window, 0.2);
			s.setPosition(pos);
			pos.x += size.x * 0.15;
		}
	}
}


void RoundVictory::update(const sf::Time& dT) {
	t -= dT;
	if (t <= sf::Time::Zero) {
		//SOUND
		if (currentSecond == 3) {
			if (characterScore == 1) {
				audioPlayer.play(AudioPlayer::Effect::WIN_ROUND_1);
			}
			else if (characterScore == 2) {
				audioPlayer.play(AudioPlayer::Effect::WIN_ROUND_2);
			}
			else {
				audioPlayer.play(AudioPlayer::Effect::WIN_GOTO_SCOREBOARD);
			}
		}
		currentSecond--;
		t = sf::seconds(1);
	}
	tickAnimation(dT);
}

void RoundVictory::tickAnimation(sf::Time dT) {
	animationCountdown -= dT;
	if (currentSecond < 3 && animationCountdown <= sf::Time::Zero) {
		if (characterVictoryPose) {
			characterVictoryPose->advance_frame(mySprite);
			mySprite.setOrigin(mySprite.getTextureRect().width / 2, mySprite.getTextureRect().height);
			mySprite.setPosition(width * 0.2f, height * 0.882f);
		}
		animationCountdown = glb::ANIMATION_PERIOD;
	}
}


bool RoundVictory::ended() const {
	return currentSecond <= 0;
}

void RoundVictory::draw(sf::RenderWindow& window) const {
	if (currentSecond < 3) {
		auto view = window.getView();
		window.setView(window.getDefaultView());
		window.draw(bgSprite);// stripes
		window.draw(mySprite);// character animation
		for (const auto& s : sprites) window.draw(s); // points
		window.setView(view);
	}
}


