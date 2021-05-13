#pragma once
#include "SFML/Graphics.hpp"

namespace particles {

struct Settings {
	sf::Vector2f pos, 
		vel = sf::Vector2f(0,0), 
		velVariation = sf::Vector2f(200, 200);
	float sizeMin = 1, sizeMax = 10;
	size_t count = 1000;
	sf::Time ttl= sf::seconds(2);
};

class Particle : sf::Transformable
{
public:
	sf::Vector2f vel;
	float rotation = 0;
	sf::Time ttl;
	float minRadius, maxRadius;
	bool active = false;

	void setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, float r, int idx); // Sets the position of the 4 vertices

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

	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void enableParticle(int idx); // enables the particle to be drawn (sets the texCoords)
	void disableParticle(int idx); // disables it
public:
	ParticleSystem(int nParticles = 1000);

	void setTexture(const sf::Texture& t);

	

	void emit(const particles::Settings& particleSettings);
	void emit(const sf::Vector2f& pos);

	void update(sf::Time elapsed);

	void clear(); // clear all particles

};

/*auto SmokeParticleSystem(const sf::Texture& t, int nParticles = 100) {
	return ParticleSystem<Particle>(t, nParticles);
}*/

} // Namespace particles