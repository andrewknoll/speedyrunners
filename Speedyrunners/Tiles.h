#pragma once
#include "SFML/Graphics.hpp"
#include <optional>

namespace Tiles {
	//const int FLOOR = 0;
	enum Collidable { // En tiles_black_editor.png
		AIR,
		FLOOR,
		JUMP_WALL_L,
		GRAPPLABLE,
		JUMP_WALL_R,
		WEIRD_SQUARES, // No se que es esto
		RAMP_UP,
		RAMP_DOWN,
		STAIRS_UP,
		STAIRS_DOWN,
		WEIRD_RAMP_UP, // Estas dos tampoco
		WEIRD_RAMP_DOWN, // "    "
		RAMP_CEIL_DOWN,
		RAMP_CEIL_UP,
		WEIRD_RAMP_CEIL_DOWN,
		WEIRD_RAMP_CEIL_UP
	};

	const int NB_COLLIDABLE = 16;

	enum Ramp {
		UP,
		DOWN,
		CEIL_UP,
		CEIL_DOWN,
		NONE
	};

	Ramp toRamp(Collidable tile);
	bool isRamp(Collidable tile);


	struct Collision {
		sf::Vector2f point; // point
		sf::Vector2f normal; // Normal
		float distance; // Distancia de entrada en el segundo obj
		Collidable tileType; // type
	};

	// Returns true if collision a has priority over b
	bool hasPriority(const Collision& a, const Collision& b);

	// Returns the collision, if any, between the hitbox of a character and a tile
	std::optional<Tiles::Collision> collision(const Collidable tile, const sf::Vector2f& tilePos, const sf::Vector2f& tileSize, const sf::FloatRect& hitbox, bool isGrounded = false);


	std::optional<Tiles::Collision> rampCollision(const Tiles::Ramp ramp, const sf::Vector2f& tilePos, const sf::Vector2f& tileSize, const sf::FloatRect& hitbox, bool isGrounded = false);

}
