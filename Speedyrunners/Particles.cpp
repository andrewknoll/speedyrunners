
#include <sstream>
#include "Particles.h"
#include "utils.hpp"

namespace particles {
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
		vertices[idx].position = pos + sf::Vector2f(-r, -r);
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
		vertices[++idx].position = pos + sf::Vector2f(-r, r);
		vertices[idx].color = c;
		vertices[idx].color.a = alpha;
		//std::cout << "idx de vertices: " << idx << "\n";
	}


	void Particle::setVertices(sf::VertexArray& vertices, const sf::Vector2f& pos, int idx, float r, float angle, float alpha, const sf::Color& c)
	{
		idx *= 4; // 4 times as many vertices as particles
		sf::Transform t;
		t.rotate(-angle);
		// top L:
		vertices[idx].position = pos + t.transformPoint(sf::Vector2f(-r , -r ));
		vertices[idx].color = c;
		vertices[idx].color.a = alpha;
		// top R:
		vertices[++idx].position = pos + t.transformPoint(sf::Vector2f(r, -r));
		vertices[idx].color = c;
		vertices[idx].color.a = alpha;
		// bottom R:
		vertices[++idx].position = pos + t.transformPoint(sf::Vector2f(r,r));
		vertices[idx].color = c;
		vertices[idx].color.a = alpha;
		// bottom L:
		vertices[++idx].position = pos + t.transformPoint(sf::Vector2f(-r, r));
		vertices[idx].color = c;
		vertices[idx].color.a = alpha;
		//std::cout << "idx de vertices: " << idx << "\n";
	}

	void Particle::setAlpha(sf::VertexArray& vertices, int idx, float a) {
		idx *= 4; // 4 times as many vertices as particles
		// top L:
		vertices[idx].color.a = a;
		// top R:
		vertices[++idx].color.a = a;
		// bottom R:
		vertices[++idx].color.a = a;
		// bottom L:
		vertices[++idx].color.a = a;
	}

	bool Particle::update(sf::Time dT, sf::VertexArray& vertices, int idx)
	{
		if (active) {
			if (ttl < sf::Time::Zero) {
				active = false;
				setAlpha(vertices, idx, 0); // hide
				return true;
			}
			else {
				float dtSec = dT.asSeconds();
				sf::Transformable::move(vel * dtSec);
				if (gravity && vel.y < physics::MAX_FALL_SPEED) vel.y += 0.4f * physics::GRAVITY * dtSec;
				sf::Transformable::rotate(rotation * dtSec);
				float radius = utils::lerp(sizeEnd, sizeIni, ttl / maxTtl);
				float alpha = utils::lerp(alphaEnd, alphaIni, ttl / maxTtl);
				float angle = 90 + utils::lerp(angleEnd, angleIni, ttl / maxTtl) * 180.0f / geometry::PI; // radians
				sf::Color c = utils::lerp(colorEnd, colorIni, ttl / maxTtl);
				//setVertices(vertices, getPosition(), idx, radius, alpha, c);
				setVertices(vertices, getPosition(), idx, radius, angle, alpha, c);
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


		maxTtl = ttl;
		gravity= particleSettings.gravity;
		sizeEnd = particleSettings.sizeEnd;
		sizeIni = particleSettings.sizeIni;
		alphaEnd = particleSettings.alphaEnd;
		alphaIni = particleSettings.alphaIni;
		angleEnd = particleSettings.angleEnd;
		angleIni = particleSettings.angleIni;
		colorEnd = particleSettings.colorEnd;
		colorIni = particleSettings.colorIni;
	}

	// Settings read from file:

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
				if (getline(is, rest) && rest != "") { // animation
					is = std::istringstream(rest);
					animate = parseInt(is, sep) == 1;
				}
			}
		}
		if (animate) std::cout << "Animated particle system\n";
	}


}