#pragma once
#include "SFML/Graphics.hpp"

namespace particles {

struct Settings {
	sf::Vector2f pos, 
		vel = sf::Vector2f(0,0), 
		velVarNeg = sf::Vector2f(20, 20), // negative vel variation in each axis
		velVarPos = sf::Vector2f(20, 20); // positive vel variation in each axis
	float sizeIni = 15, sizeEnd = 50;
	float alphaIni = 255, alphaEnd = 0;
	size_t count = 1000;
	sf::Time ttl= sf::seconds(2);


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

	void setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float alpha = 255);

	bool update(sf::Time dT, const Settings& pSettings, sf::VertexArray& vertices, int idx);

	bool update(sf::Time dT, sf::Time maxTtl, sf::VertexArray& vertices, int idx); // returns true if it has died
	void reset(const particles::Settings& particleSettings);

};



//template <typename Particle>
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
protected:
	Settings pSettings;

	const sf::Texture* tex;

	std::vector<Particle> particles;
	size_t index;
	sf::VertexArray vertices;

	sf::Vector2u texSize;
	sf::IntRect texRect;

	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void enableParticle(int idx); // enables the particle to be drawn (sets the texCoords)
	void disableParticle(int idx); // disables it
public:
	ParticleSystem(const Settings& particleSettings);

	void setTexture(const sf::Texture& t, const sf::FloatRect& rect=sf::FloatRect(0,0,1,1));

	void setParticleSettings(Settings& particleSettings);

	void emit(const particles::Settings& particleSettings);
	void emit(const sf::Vector2f& pos);

	void update(sf::Time elapsed);

	void clear(); // clear all particles

};

/*auto SmokeParticleSystem(const sf::Texture& t, int nParticles = 100) {
	return ParticleSystem<Particle>(t, nParticles);
}*/

} // Namespace particles