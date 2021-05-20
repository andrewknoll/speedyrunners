#pragma once
#include "SFML/Graphics.hpp"
#include "Particles.h"

namespace particles {



//template <typename Particle>

//template <typename P>
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
protected:
	particles::Settings pSettings;

	const sf::Texture* tex;

	std::vector<Particle> particles;
	size_t index;
	sf::VertexArray vertices;

	sf::Vector2u texSize;
	sf::Vector2i nSprites = sf::Vector2i(1,1);
	sf::IntRect texRect;

	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void enableParticle(int idx); // enables the particle to be drawn (sets the texCoords)
	void enableParticleRandomSprite(int idx); // enables the particle to be drawn (sets the texCoords) with a random rect
	void enableParticle(int idx, int spriteIdx); // enables the particle to be drawn (sets the texCoords) with a random rect
	void disableParticle(int idx); // disables it
public:
	ParticleSystem(const particles::Settings& particleSettings);

	void setTexture(const sf::Texture& t, const sf::FloatRect& rect=sf::FloatRect(0,0,1,1));

	void setParticleSettings(particles::Settings& particleSettings);
	particles::Settings getSettings() const;

	void emit(const particles::Settings& particleSettings);
	void emit(const sf::Vector2f& pos);
	void emit(const sf::Vector2f& pos, const sf::Vector2f& dir);

	// emits nParticles around center, maximum radius distance
	void burst(const sf::Vector2f& center, const float& radius, int nParticles);

	// emits nParticles around center, maximum radius distance, all particles are emited facing outwards
	void burstOut(const sf::Vector2f& center, const float& radius, int nParticles);

	// fills a line with particles
	void emitLinear(const sf::Vector2f& origin, const float& distance);

	// emits nParticles in a line
	void emitLinear(const sf::Vector2f& origin, const float& distance, int nParticles);


	void update(sf::Time elapsed);

	void clear(); // clear all particles

	void setAllParticlesPosition(sf::Vector2f pos);

};

using PSystem = ParticleSystem;

/*auto SmokeParticleSystem(const sf::Texture& t, int nParticles = 100) {
	return ParticleSystem<Particle>(t, nParticles);
}*/

} // Namespace particles
