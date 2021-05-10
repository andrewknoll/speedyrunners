#include <iostream>
#include "Background.h"

Background::Background(const std::string& file, const sf::RenderWindow& window) {
	load(file, window);
}

Background::Background(const std::string& file, const sf::RenderWindow& window, const sf::FloatRect& zona)
{
	load(file, window, zona);
}


void Background::load(const std::string& file, const sf::RenderWindow& window) {
	sf::Vector2u windowSize = window.getSize();
	//sf::Vector2u windowSize = window.mapCoordsToPixel(
	//	window.getSize());
	if (!bgTexture.loadFromFile(file))
	{
		std::cerr << "Error cargando fondo de " << file << "\n";
	}
	else {
		// Vertices for background:
		bgVertices.setPrimitiveType(sf::Quads);
		bgVertices.resize(4);
		// define its 4 corners
		bgVertices[0].position = sf::Vector2f(0, 0);
		bgVertices[1].position = sf::Vector2f(windowSize.x, 0);
		bgVertices[2].position = sf::Vector2f(windowSize.x, windowSize.y);
		bgVertices[3].position = sf::Vector2f(0, windowSize.y);

		// define its 4 texture coordinates
		auto texSize = bgTexture.getSize();
		bgVertices[0].texCoords = sf::Vector2f(0, 0);
		bgVertices[1].texCoords = sf::Vector2f(texSize.x, 0);
		bgVertices[2].texCoords = sf::Vector2f(texSize.x, texSize.y);
		bgVertices[3].texCoords = sf::Vector2f(0, texSize.y);
		currentTexSize = sf::Vector2f(texSize.x, texSize.y);

	}
}


void Background::load(const std::string& file, const sf::RenderWindow& window, const sf::FloatRect& zona) {
	sf::Vector2u windowSize = window.getSize();
	//sf::Vector2u windowSize = window.mapCoordsToPixel(
	//	window.getSize());
	if (!bgTexture.loadFromFile(file))
	{
		std::cerr << "Error cargando fondo de " << file << "\n";
	}
	else {
		// Vertices for background:
		bgVertices.setPrimitiveType(sf::Quads);
		bgVertices.resize(4);
		// define its 4 corners
		bgVertices[0].position = sf::Vector2f(zona.left*windowSize.x, zona.top*windowSize.y);
		bgVertices[1].position = sf::Vector2f((zona.left+zona.width)*windowSize.x, zona.top * windowSize.y);
		bgVertices[2].position = sf::Vector2f((zona.left + zona.width) * windowSize.x, (zona.top + zona.height) * windowSize.y);
		bgVertices[3].position = sf::Vector2f(zona.left * windowSize.x, (zona.top + zona.height) * windowSize.y);

		// define its 4 texture coordinates
		auto texSize = bgTexture.getSize();
		bgVertices[0].texCoords = sf::Vector2f(0, 0);
		bgVertices[1].texCoords = sf::Vector2f(texSize.x, 0);
		bgVertices[2].texCoords = sf::Vector2f(texSize.x, texSize.y);
		bgVertices[3].texCoords = sf::Vector2f(0, texSize.y);
		currentTexSize  = sf::Vector2f(texSize.x, texSize.y);

	}
}

void Background::setTextureCoords(const sf::FloatRect& zona) {

	auto texSize = bgTexture.getSize();
	bgVertices[0].texCoords = sf::Vector2f(zona.left * texSize.x, zona.top * texSize.y);
	bgVertices[1].texCoords = sf::Vector2f((zona.left + zona.width) * texSize.x, zona.top * texSize.y);
	bgVertices[2].texCoords = sf::Vector2f((zona.left + zona.width) * texSize.x, (zona.top + zona.height) * texSize.y);
	bgVertices[3].texCoords = sf::Vector2f(zona.left * texSize.x, (zona.top + zona.height) * texSize.y);

	currentTexSize = sf::Vector2f(bgVertices[2].texCoords.x - bgVertices[0].texCoords.x, bgVertices[2].texCoords.y - bgVertices[0].texCoords.y);
}

void Background::fixProportions() {
	float ratio = currentTexSize.x / currentTexSize.y;
	float width = bgVertices[1].position.x - bgVertices[0].position.x;
	float ymax = bgVertices[0].position.y + width / ratio; // Recalculate the ymax coordinate respecting the original texture proportions
	bgVertices[2].position.y = ymax;
	bgVertices[3].position.y = ymax;
	
#ifdef DEBUG_PROPORTIONS // check fix:
	float currW = bgVertices[2].position.x - bgVertices[0].position.x;
	float currH = bgVertices[2].position.y - bgVertices[0].position.y;
	if (currW / currH != ratio)
		std::cerr << "Nope: " << currW << " / " << currH << " = " << currW / currH << "!=" << ratio << "\n";
#endif
}



void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto view = target.getView();
	target.setView(target.getDefaultView()); // background shouldnt be moved
	// apply the tileset texture
	states.texture = &bgTexture;
	states.transform = sf::Transform();
	// draw the vertex array
	target.draw(bgVertices, states);
	target.setView(view);
}