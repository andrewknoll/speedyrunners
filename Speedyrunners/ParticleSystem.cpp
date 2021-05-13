#include <iostream>
#include "ParticleSystem.h"
#include "utils.hpp"
#include "Resources.h"

namespace particles {

	ParticleSystem::ParticleSystem(int nParticles) 
		: particles(nParticles), vertices(sf::Quads, nParticles *4), index(nParticles-1), tex(nullptr)
	{
		std::cout << "index: " << index << "\n";
		particles.resize(nParticles);
		pSettings.count = nParticles;
	}

	void ParticleSystem::setTexture(const sf::Texture& t)
	{
		tex = &t;
		auto size = t.getSize();
		for (int i = 0; i < vertices.getVertexCount(); i++) {
			vertices[i++].texCoords = sf::Vector2f(0, 0);	   vertices[i++].texCoords = sf::Vector2f(size.x, 0);
			vertices[i++].texCoords = sf::Vector2f(0, size.y); vertices[i].texCoords = sf::Vector2f(size.x, size.y);
		}
	}


	void ParticleSystem::emit(const particles::Settings& particleSettings)
	{
		pSettings = particleSettings;
		particles[index].reset(pSettings);
		if (index == 0) index = particles.size() - 1;
		else index--;
	}

	void ParticleSystem::emit(const sf::Vector2f& pos)
	{
		std::cout << "emit index: " << index << "\n";
		pSettings.pos = pos;
		particles[index].reset(pSettings);
		if (index == 0) index = particles.size() - 1;
		else index--;
	}


	void ParticleSystem::update(sf::Time elapsed)
	{
		int i = 0;
		for (auto& p : particles) {
			p.update(elapsed, pSettings.ttl, vertices, i);
			i++;
		}
	}

	void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		states.texture = tex;

		target.draw(vertices, states);
	}

	void Particle::setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, float r, int idx)
	{
		idx *= 4; // 4 times as many vertices as particles
		vertices[idx++].position = pos + sf::Vector2f(-r,-r); vertices[idx++].position = pos + sf::Vector2f(r, -r);
		vertices[idx++].position = pos + sf::Vector2f(-r, r); vertices[idx].position = pos + sf::Vector2f(r, r);
		std::cout << "idx de vertices: " << idx << "\n";
	}

	void Particle::update(sf::Time dT, sf::Time maxTtl, sf::VertexArray& vertices, int idx)
	{
		if (active) {
			if (ttl < sf::Time::Zero) active = false;
			else {
				ttl -= dT;
				float dtSec = dT.asSeconds();
				sf::Transformable::move(vel * dtSec);
				sf::Transformable::rotate(rotation * dtSec);
				float radius = utils::lerp(minRadius, maxRadius, ttl / maxTtl);
				setVertices(vertices, getPosition(), radius, idx);
			}
		}
	}

	void Particle::reset(const particles::Settings& particleSettings)
	{
		setPosition(particleSettings.pos);
		vel = particleSettings.vel;
		auto& var = particleSettings.velVariation;
		vel += 0.5f * sf::Vector2f(rng::defaultGen.rand(-var.x, var.x), rng::defaultGen.rand(-var.y, var.y));
		minRadius = particleSettings.sizeIni;
		maxRadius = particleSettings.sizeEnd;
		ttl = particleSettings.ttl;
		active = true;
		setRotation(rng::defaultGen.rand(0, 360));
	}

}// Namespace particles
