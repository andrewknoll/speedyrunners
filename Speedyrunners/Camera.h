#pragma once
#include "Character.h"
#include "SFML/Graphics.hpp"


class Camera : public sf::View, public sf::Drawable
{
	using CharPtr = std::shared_ptr<Character>;
	//sf::View view;

private:
	const float EPSILON = 0.01f;
	float rectSizeFactor = 1.0f;
	
	bool dragging = false;
	bool suddenDeath = false;
	bool increasingRedness = true;
	
	sf::RectangleShape viewportShape;
	
	sf::Uint16 redValue = 0;

	sf::Vector2i lastMousePos;
	sf::Vector2f size0;
	sf::Vector2f objectivePos, objectiveSize;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Camera(const sf::View& v);

	Camera(const sf::FloatRect& rect = sf::FloatRect(0,0,10,10));
	//sf::Rect<float> view;

	void moveByMouse(sf::Vector2i pos);

	void resetDrag();
	
	void operator= (const sf::View& v);

	void immediateFollow(std::vector<CharPtr>& characters, int first = 0);

	void follow(std::vector<CharPtr>& characters, int first = 0);

	sf::FloatRect viewRectangle() const;

	bool isInAllowedBounds(CharPtr character) const;

	//void emitDeathParticles(bool multipleExplosions = false);
	sf::Vector2f closestInView(const sf::Vector2f& p) const; // closest point to p in view

	void setRect(const sf::FloatRect& rect);
	void setSuddenDeath(bool sd);
	void update(sf::Time dT);
};