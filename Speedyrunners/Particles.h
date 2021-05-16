#pragma once
#include "SFML/Graphics.hpp"
namespace particles{

	struct Settings {
		sf::Vector2f pos,
			vel = sf::Vector2f(0, 0),
			velVarNeg = sf::Vector2f(20, 20), // negative vel variation in each axis
			velVarPos = sf::Vector2f(20, 20); // positive vel variation in each axis
		float sizeIni = 15, sizeEnd = 50;
		float alphaIni = 255, alphaEnd = 0;
		size_t count = 1000;
		sf::Time ttl = sf::seconds(2);
		bool randomSprites = false;
		bool gravity = false;
		sf::Color colorIni = sf::Color::White,
			colorEnd = sf::Color::White;

		bool animate = false;

		float angleIni = 0, angleEnd = 0;

		Settings(const std::string& line, const char sep = ','); // loads the settings from a line containing the values separated by 'sep'

	};


	class Particle : sf::Transformable
	{
	public:
		sf::Vector2f vel;
		float rotation = 0;
		sf::Time ttl;
		float minRadius, maxRadius;
		bool active = false;


		sf::Time maxTtl;
		bool gravity;
		float sizeEnd, sizeIni,
			alphaEnd, alphaIni,
			angleEnd, angleIni;
		sf::Color colorEnd, colorIni;
		/*int currentFrame = 0, maxFrames;

		sf::Time animCD = sf::Time::Zero; // animations
		*/
		void setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float alpha = 255);

		void setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float alpha, const sf::Color& c);

		void setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float angle, float alpha, const sf::Color& c);

		void setAlpha(sf::VertexArray& vertices, int idx, float a);

		bool update(sf::Time dT, sf::VertexArray& vertices, int idx);

		bool update(sf::Time dT, const Settings& pSettings, sf::VertexArray& vertices, int idx);// returns true if it has died

		void reset(const particles::Settings& particleSettings);

	};


} // Namespace particles