#include "Tiles.h"
#include "utils.hpp"

Tiles::Ramp Tiles::toRamp(Collidable tile)
{
	using namespace Tiles;
	if (tile == Collidable::RAMP_DOWN || tile == Collidable::STAIRS_DOWN) {
		return DOWN;
	}
	else if (tile == Collidable::RAMP_UP || tile == Collidable::STAIRS_UP) {
		return UP;
	}
	/*else if (tile == Collidable::RAMP_CEIL_DOWN) {
		return CEIL_DOWN;
	}
	else if (tile == Collidable::RAMP_CEIL_UP) {
		return CEIL_UP;
	}*/
	else {
		return Ramp::NONE;
	}
}

bool Tiles::isRamp(Collidable tile)
{
	return toRamp(tile) != Tiles::Ramp::NONE;
}



/*if (true || hDif > 0) {
				// Collision
				sf::Vector2f point = downR; // not exact
				sf::Vector2f n = geometry::normalize(sf::Vector2f(-1, -1));
				std::cout << "collided with ramp\n";
				return Tiles::Collision{ point, n, hDif/sqrt(2.0f)/2.0f }; // hDif is actually vertical distance, not normal one
			}*/
#ifdef OLD_RAMP_COLLISION
std::optional<Tiles::Collision> Tiles::rampCollision(const Tiles::Ramp ramp, const sf::Vector2f& tilePos,
	const sf::Vector2f& tileSize, const sf::FloatRect& hitbox)
{
	std::cout << "colliding ramp: " << ramp << "\n";
	if (ramp == Tiles::Ramp::UP) {
		// Ramp up
		sf::Vector2f downR(hitbox.left + hitbox.width, hitbox.top + hitbox.height); // down right point of the hitbox
		float dist;
		// Collision
		sf::Vector2f point = downR; // not exact
		sf::Vector2f n = geometry::normalize(sf::Vector2f(0, -1));
		if (downR.x < tilePos.x + tileSize.x) {
			float hDif = (downR.y - (tilePos.y - (downR.x - tilePos.x))); // diff in height
			std::cout << "hdif: " << hDif << "\n";

			//std::cout << "collided with ramp\n";
			 // hDif is actually vertical distance, not normal one
			dist = hDif;
		}
		else { // No collision
			dist = downR.y - tilePos.y;
		}
		return Tiles::Collision{ point, n, dist };
	}
	else {
		// TODO: los otros 4 tipos de rampas
		return {};
	}
}
#else
std::optional<Tiles::Collision> Tiles::rampCollision(const Tiles::Ramp ramp, const sf::Vector2f& tilePos,
	const sf::Vector2f& tileSize, const sf::FloatRect& hitbox, bool isGrounded)
{
	//std::cout << "colliding ramp: " << ramp << "\n";
	if (ramp == Tiles::Ramp::UP || ramp == Tiles::Ramp::DOWN) {

		// Ramp up
		sf::Vector2f downCenter(hitbox.left + hitbox.width / 2.0f, hitbox.top + hitbox.height); // down center point of the hitbox
		float dist = 0;
		sf::Vector2f n = geometry::normalize(sf::Vector2f(0, -1));
		if (downCenter.x > tilePos.x && downCenter.x < tilePos.x + tileSize.x) {
			if (ramp == Tiles::Ramp::DOWN) downCenter.x = tilePos.x + tileSize.x - (downCenter.x - tilePos.x); // flip horizontally, the rest is the same
			sf::Vector2f target = downCenter;
			target.y = tilePos.y + tileSize.y - (downCenter.x - tilePos.x);
			dist = target.y - downCenter.y;//(downCenter.y - (tilePos.y  + (downCenter.x - tilePos.x - tileSize.y))); // diff in height
			/*std::cout << downCenter.x - tilePos.x << "\n-tilepos.y = " << -tilePos.y + (downCenter.x - tilePos.x)
			   << "\n+downCenter.y = " << dist;*/
		}
		else return {};
		/*else if (downCenter.x > tilePos.x + tileSize.x) { // No collision
			dist = downCenter.y - tilePos.y;
			//dist = 0;
		}*/

		//sf::sleep(sf::seconds(2));
		if (!isGrounded && dist > 0) return {};
		return Tiles::Collision{ downCenter, n / utils::length(n), -dist };
	}
	else {
		return {};
	}
}
#endif

bool Tiles::hasPriority(const Collision& a, const Collision& b)
{
	if (isRamp(a.tileType) && !isRamp(b.tileType)) return true;
	else if (!isRamp(a.tileType) && isRamp(b.tileType)) return false;
	return a.distance > b.distance;
}

// Adapted from the SAT method in https://laptrinhx.com/custom-physics-engine-part-2-manifold-generation-716517698/
std::optional<Tiles::Collision> Tiles::collision(const Tiles::Collidable tile, const sf::Vector2f& tilePos,
	const sf::Vector2f& tileSize, const sf::FloatRect& hitbox, bool isGrounded)
{
	sf::FloatRect tileRect(tilePos.x, tilePos.y, tileSize.x, tileSize.y);
	// x overlap:
	float a_extent = (tileRect.width) / 2;
	float b_extent = (hitbox.width) / 2;
	float x_overlap = a_extent + b_extent - std::abs(hitbox.left + b_extent - (tileRect.left + a_extent)); // std::abs(hitbox.left-tileRect.left);
	// y overlap:
	a_extent = (tileRect.height) / 2;
	b_extent = (hitbox.height) / 2;
	float y_overlap = a_extent + b_extent - std::abs(hitbox.top + b_extent - (tileRect.top + a_extent));
	if (x_overlap <= 0 || y_overlap <= 0) { // No overlap, no collision
		/*if (x_overlap <= 0) std::cout << "no x overlap\n";
		else std::cout << "no y overlap\n";*/
		return {};
	} // Here we have a collision:
	// if its a ramp:
	if (Tiles::toRamp(tile) != NONE) return rampCollision(Tiles::toRamp(tile), tilePos, tileSize, hitbox);
	// Otherwise, its a square tile:
	// collision points:
	sf::Vector2f collision1(std::max(tileRect.left, hitbox.left), std::max(tileRect.top, hitbox.top));
	sf::Vector2f collision2(std::min(tileRect.left + tileRect.width, hitbox.left + hitbox.width),
		std::min(tileRect.top + tileRect.height, hitbox.top + hitbox.height));


	sf::Vector2f n; // Normal
	sf::Vector2f point; // Point
	float dist; // Distance
	if (x_overlap < y_overlap)
	{
		// Point towards B knowing that t points from A to B
		n = (hitbox.left - tileRect.left) < 0 ? sf::Vector2f(-1, 0) : sf::Vector2f(1, 0);
		dist = x_overlap;
		point = collision1; // TODO: This is wrong! -> we have to return both points i think
	}
	else
	{
		// Point toward B knowing that t points from A to B
		n = (hitbox.top - tileRect.top) < 0 ? sf::Vector2f(0, -1) : sf::Vector2f(0, 1);
		dist = y_overlap;
		point = collision2;
	}
	//std::cout << x_overlap << " " << y_overlap << "\n";
	/*if (tile == Collidable::GRAPPLABLE) {
		std::cout << "collision with grapplable: " << n << " " << dist << "\n";
	}*/

	return Tiles::Collision{ point, n / utils::length(n), dist };// TODO: This is wrong!
}