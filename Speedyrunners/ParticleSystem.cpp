#include <iostream>
#include "ParticleSystem.h"
#include "utils.hpp"
#include "Resources.h"



namespace particles {

	/*ParticleSystem::ParticleSystem(int nParticles) 
		: particles(nParticles), vertices(sf::Quads, nParticles *4), index(nParticles-1), tex(nullptr)
	{
		std::cout << "index: " << index << "\n";
		particles.resize(nParticles);
		pSettings.count = nParticles;
	}*/
	
	ParticleSystem::ParticleSystem(const Settings& particleSettings)
		: pSettings(particleSettings), particles(particleSettings.count), vertices(sf::Quads, particleSettings.count * 4), index(particleSettings.count - 1), tex(nullptr)
	{
		particles.resize(particleSettings.count);
	}

	
	void ParticleSystem::setTexture(const sf::Texture& t, const sf::FloatRect& rect)
	{
		tex = &t;
		texSize = t.getSize();
		texRect = sf::IntRect(texSize.x * rect.left, texSize.y * rect.top, texSize.x * rect.width, texSize.y * rect.height);
		nSprites = utils::spritesForFloatRect(rect);
		/*for (int i = 0; i < particles.size(); i+=4) {
			enableParticle(i);
		}*/
	}

	
	void ParticleSystem::setParticleSettings(particles::Settings& particleSettings)
	{
		pSettings = particleSettings;
	}

	particles::Settings ParticleSystem::getSettings() const
	{
		return pSettings;
	}

	
	void ParticleSystem::enableParticle(int idx) {
		idx *= 4;
		vertices[idx++].texCoords = sf::Vector2f(texRect.left, texRect.top); // top L	
		vertices[idx++].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top); // top R
		vertices[idx++].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top + texRect.height); // bottom R
		vertices[idx].texCoords   = sf::Vector2f(texRect.left, texRect.top + texRect.height); // bottom L
	}

	
	void ParticleSystem::enableParticleRandomSprite(int idx)
	{
		int spriteIdx = rng::defaultGen.rand(0, nSprites.x * nSprites.y);
		enableParticle(idx, spriteIdx);
	}

	void ParticleSystem::enableParticle(int idx, int spriteIdx)
	{
		int xIdx = spriteIdx % nSprites.x;
		int yIdx = spriteIdx / nSprites.x;
		idx *= 4;
		vertices[idx++].texCoords = sf::Vector2f(texRect.width * xIdx, texRect.height * yIdx); // top L	
		vertices[idx++].texCoords = sf::Vector2f(texRect.width * (xIdx + 1), texRect.height * yIdx); // top R
		vertices[idx++].texCoords = sf::Vector2f(texRect.width * (xIdx + 1), texRect.height * (yIdx + 1)); // bottom R
		vertices[idx].texCoords = sf::Vector2f(texRect.width * xIdx, texRect.height * (yIdx + 1)); // bottom L
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
		// enable the vertices:
		if (pSettings.randomSprites) enableParticleRandomSprite(index);
		else enableParticle(index); 
		if (index == 0) index = particles.size() - 1;
		else index--;
	}

	void ParticleSystem::emit(const sf::Vector2f& pos, const sf::Vector2f& dir)
	{
		pSettings.pos = pos;
		pSettings.angleIni = geometry::PI/2.0f + atan2f(dir.x, dir.y);
		pSettings.angleEnd = geometry::PI/2.0f + atan2f(dir.x, dir.y);
		particles[index].reset(pSettings);
		// enable the vertices:
		if (pSettings.randomSprites) enableParticleRandomSprite(index);
		else enableParticle(index);
		if (index == 0) index = particles.size() - 1;
		else index--;
	}

	void ParticleSystem::burst(const sf::Vector2f& center, const float& radius, int nParticles)
	{
		for (int i = 0; i < nParticles; i++)
		{
			float angle = rng::defaultGen.rand(0.0f, 2.0f * geometry::PI);
			float dist = rng::defaultGen.rand(0.0f, radius);
			sf::Vector2f dir(cos(angle), sin(angle));
			emit(center + (dist * dir));
		}
	}

	void ParticleSystem::burstOut(const sf::Vector2f& center, const float& radius, int nParticles)
	{
		for (int i = 0; i < nParticles; i++)
		{
			float angle = rng::defaultGen.rand(0.0f, 2.0f * geometry::PI);
			float dist = rng::defaultGen.rand(0.0f, radius);
			sf::Vector2f dir(cos(angle), sin(angle));
			emit(center + (dist * dir), dir);
		}
	}

	
	void ParticleSystem::update(sf::Time elapsed)
	{
		int i = 0;
		for (auto& p : particles) {
			if (p.update(elapsed, vertices, i)) // the particle has died
				;// nothing needed i think
			else if (pSettings.animate) {
				int frame = utils::lerp(nSprites.x * nSprites.y - 1, 0, p.ttl/pSettings.ttl);
				enableParticle(i, frame);
			}
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
	

}// Namespace particles
