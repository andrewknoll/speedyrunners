#pragma once
#include "SFML/Graphics.hpp"
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

	ParticleSystem(unsigned int count);

	void setEmitter(sf::Vector2f position);

	void update(sf::Time elapsed);

private:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
	};

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;

	void resetParticle(std::size_t index);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

