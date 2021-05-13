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

	void ParticleSystem::setTexture(const sf::Texture& t, const sf::FloatRect& rect)
	{
		tex = &t;
		texSize = t.getSize();
		texRect = sf::IntRect(texSize.x * rect.left, texSize.y * rect.top, texSize.x * rect.width, texSize.y * rect.height);
		/*for (int i = 0; i < particles.size(); i+=4) {
			enableParticle(i);
		}*/
	}

	void ParticleSystem::enableParticle(int idx) {
		idx *= 4;
		vertices[idx++].texCoords = sf::Vector2f(texRect.left, texRect.top);//sf::Vector2f(0, 0); // top L	
		vertices[idx++].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top); //sf::Vector2f(texSize.x, 0); // top R
		vertices[idx++].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top + texRect.height); //sf::Vector2f(texSize.x, texSize.y); // bottom R
		vertices[idx].texCoords   = sf::Vector2f(texRect.left, texRect.top + texRect.height); // bottom L
	}
	void ParticleSystem::disableParticle(int idx) {
		idx *= 4;
		sf::Vector2f empty(0, 0);
		vertices[idx++].texCoords = empty; // top L	
		vertices[idx++].texCoords = empty; // top R
		vertices[idx++].texCoords = empty; // bottom R
		vertices[idx].texCoords   = empty; // bottom L
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
		//std::cout << "emit index: " << index << "\n";
		pSettings.pos = pos;
		particles[index].reset(pSettings);
		enableParticle(index); // enable the vertices
		if (index == 0) index = particles.size() - 1;
		else index--;
	}


	void ParticleSystem::update(sf::Time elapsed)
	{
		int i = 0;
		for (auto& p : particles) {
			if (p.update(elapsed, pSettings.ttl, vertices, i)) // the particle has died
				disableParticle(i); // stop drawing it
			i++;
		}
	}

	void ParticleSystem::clear()
	{
		for (auto& p : particles) p.active = false;
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
		vertices[idx++].position = pos + sf::Vector2f(-r,-r); // top L
		vertices[idx++].position = pos + sf::Vector2f(r, -r); // top R
		vertices[idx++].position = pos + sf::Vector2f(r, r);  // bottom R
		vertices[idx].position	 = pos + sf::Vector2f(-r, r); // bottom L
		//std::cout << "idx de vertices: " << idx << "\n";
	}

	bool Particle::update(sf::Time dT, sf::Time maxTtl, sf::VertexArray& vertices, int idx)
	{
		if (active) {
			if (ttl < sf::Time::Zero) {
				active = false;
				return true;
			}
			else {
				ttl -= dT;
				float dtSec = dT.asSeconds();
				sf::Transformable::move(vel * dtSec);
				sf::Transformable::rotate(rotation * dtSec);
				float radius = utils::lerp(minRadius, maxRadius, ttl / maxTtl);
				setVertices(vertices, getPosition(), radius, idx);
			}
		}
		return false;
	}

	void Particle::reset(const particles::Settings& particleSettings)
	{
		setPosition(particleSettings.pos);
		vel = particleSettings.vel;
		auto& var = particleSettings.velVariation;
		vel += 0.5f * sf::Vector2f(rng::defaultGen.rand(-var.x, var.x), rng::defaultGen.rand(-var.y, var.y));
		minRadius = particleSettings.sizeMin;
		maxRadius = particleSettings.sizeMax;
		ttl = particleSettings.ttl;
		active = true;
		setRotation(rng::defaultGen.rand(0, 360));
	}

}// Namespace particles
