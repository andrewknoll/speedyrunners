/*#include "SmokeParticle.h"
#include "Resources.h"
#include "utils.hpp"

SmokeParticle::SmokeParticle()
{
	auto& t = Resources::getInstance().getMiscTexture(8);
	setTexture(t);
	setTextureRect(utils::relativeToGlobalTextureRect(sf::FloatRect(0,0,0.5,1),t));
	utils::centerOrigin(*this);
}


void SmokeParticle::reset(const ParticleSystem<SmokeParticle>::Settings& settings) {

	ttl = settings.ttl;
	utils::setWidth(*this, settings.size); // Set the size of the particle
	setRotation(rng::gen.rand(0, 360)); // Random angle
	setPosition(settings.pos);
}



for (std::size_t i = 0; i < particles.size(); ++i)
{
	// update the particle lifetime
	Particle& p = particles[i];
	p.ttl -= elapsed;

	// if the particle is dead, respawn it
	if (p.ttl <= sf::Time::Zero)
		continue;
	//resetParticle(i);


	vertices[i].position += p.velocity * elapsed.asSeconds();

	float lifeRatio = pSettings.ttl / p.ttl; // 0 to 1, for interpolation
	//vertices[i].color = utils::lerp(pSettings.colorIni, pSettings.colorEnd, lifeRatio);
	// update the alpha (transparency) of the particle according to its lifetime
	//vertices[i].color.a = utils::lerp(0, 255, lifeRatio); // decreases linearly with lifetime
}*/