#pragma once
#include "SFML/Graphics.hpp"

namespace particles {

struct Settings {
	sf::Vector2f pos, 
		vel = sf::Vector2f(0,0), 
		velVariation = sf::Vector2f(0, 0);
	float sizeIni = 100, sizeEnd = 10;
	size_t count = 100;
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

	void update(sf::Time dT, sf::Time maxTtl, sf::VertexArray& vertices, int idx);
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

	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	ParticleSystem(int nParticles = 100);

	void setTexture(const sf::Texture& t);

	void emit(const particles::Settings& particleSettings);
	void emit(const sf::Vector2f& pos);

	void update(sf::Time elapsed);

};

/*auto SmokeParticleSystem(const sf::Texture& t, int nParticles = 100) {
	return ParticleSystem<Particle>(t, nParticles);
}*/

} // Namespace particles