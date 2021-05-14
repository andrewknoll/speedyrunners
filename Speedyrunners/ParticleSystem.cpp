#include <iostream>
#include <sstream>
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

	void ParticleSystem::setParticleSettings(Settings& particleSettings)
	{
		pSettings = particleSettings;
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
		int xIdx = rng::defaultGen.rand(0, nSprites.x);
		int yIdx = rng::defaultGen.rand(0, nSprites.y);

		idx *= 4;
		vertices[idx++].texCoords = sf::Vector2f(texRect.width * xIdx, texRect.height * yIdx); // top L	
		vertices[idx++].texCoords = sf::Vector2f(texRect.width * (xIdx + 1), texRect.height * yIdx); // top R
		vertices[idx++].texCoords = sf::Vector2f(texRect.width * (xIdx + 1), texRect.height * (yIdx+1)); // bottom R
		vertices[idx].texCoords	  = sf::Vector2f(texRect.width * xIdx, texRect.height * (yIdx + 1)); // bottom L

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


	void ParticleSystem::update(sf::Time elapsed)
	{
		int i = 0;
		for (auto& p : particles) {
			if (p.update(elapsed, pSettings, vertices, i)) // the particle has died
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
	void Particle::setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float alpha)
	{
		idx *= 4; // 4 times as many vertices as particles
		// top L:
		vertices[idx].position = pos + sf::Vector2f(-r, -r);
		vertices[idx].color.a = alpha;
		// top R:
		vertices[++idx].position = pos + sf::Vector2f(r, -r);
		vertices[idx].color.a = alpha;
		// bottom R:
		vertices[++idx].position = pos + sf::Vector2f(r, r);
		vertices[idx].color.a = alpha;
		// bottom L:
		vertices[++idx].position = pos + sf::Vector2f(-r, r);
		vertices[idx].color.a = alpha;
		//std::cout << "idx de vertices: " << idx << "\n";
	}
	void Particle::setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float alpha, const sf::Color& c)
	{
		idx *= 4; // 4 times as many vertices as particles
		// top L:
		vertices[idx].position = pos + sf::Vector2f(-r,-r); 
		vertices[idx].color = c; 
		vertices[idx].color.a = alpha; 
		// top R:
		vertices[++idx].position = pos + sf::Vector2f(r, -r); 
		vertices[idx].color = c; 
		vertices[idx].color.a = alpha;
		// bottom R:
		vertices[++idx].position = pos + sf::Vector2f(r, r);  
		vertices[idx].color = c; 
		vertices[idx].color.a = alpha;
		// bottom L:
		vertices[++idx].position	 = pos + sf::Vector2f(-r, r);
		vertices[idx].color = c; 
		vertices[idx].color.a = alpha;
		//std::cout << "idx de vertices: " << idx << "\n";
	}

	bool Particle::update(sf::Time dT, const Settings& pSettings, sf::VertexArray& vertices, int idx)
	{
		if (active) {
			if (ttl < sf::Time::Zero) {
				active = false;
				return true;
			}
			else {
				float dtSec = dT.asSeconds();
				sf::Transformable::move(vel * dtSec);
				if (pSettings.gravity && vel.y < physics::MAX_FALL_SPEED) vel.y += 0.4f * physics::GRAVITY * dtSec;
				sf::Transformable::rotate(rotation * dtSec);
				sf::Time maxTtl = pSettings.ttl;
				float radius = utils::lerp(pSettings.sizeEnd, pSettings.sizeIni, ttl / maxTtl);
				float alpha = utils::lerp(pSettings.alphaEnd, pSettings.alphaIni, ttl / maxTtl);
				sf::Color c = utils::lerp(pSettings.colorEnd, pSettings.colorIni, ttl / maxTtl);
				setVertices(vertices, getPosition(), idx, radius, alpha, c);
				ttl -= dT;
			}
		}
		return false;
	}

	void Particle::reset(const particles::Settings& particleSettings)
	{
		setPosition(particleSettings.pos);
		vel = particleSettings.vel;
		auto& negVar = particleSettings.velVarNeg;
		auto& posVar = particleSettings.velVarPos;
		vel += sf::Vector2f(rng::defaultGen.rand(-negVar.x, posVar.x), rng::defaultGen.rand(-negVar.y, posVar.y));
		ttl = particleSettings.ttl;
		active = true;
		setRotation(rng::defaultGen.rand(0, 360));
	}


	

	Settings::Settings(const std::string& line, const char sep)
	{
		using namespace utils;
		std::istringstream is(line);
		pos.x = parseFloat(is, sep);
		pos.y = parseFloat(is, sep);
		vel.x = parseFloat(is, sep);
		vel.y = parseFloat(is, sep);
		velVarNeg.x = parseFloat(is, sep);
		velVarNeg.y = parseFloat(is, sep);
		velVarPos.x = parseFloat(is, sep);
		velVarPos.y = parseFloat(is, sep);
		sizeIni = parseFloat(is, sep);
		sizeEnd = parseFloat(is, sep);
		alphaIni = parseFloat(is, sep);
		alphaEnd = parseFloat(is, sep);
		count = size_t(parseInt(is, sep));
		ttl = sf::seconds(parseFloat(is, sep));
		randomSprites = parseInt(is, sep) == 1;
		std::string rest;
		if (getline(is, rest) && rest != "") { // There is gravity
			is = std::istringstream(rest);
			gravity = parseInt(is, sep) == 1;
			if (getline(is, rest) && rest != "") { // There are colors
				is = std::istringstream(rest);
				colorIni = parseColor(is, sep);
				colorEnd = parseColor(is, sep);
			}
		}
	}

}// Namespace particles
