#pragma once
#include "Character.h"
#include "Player.h"
#include "PriorityQueue.h"
#include "TileMap.h"
#include "Line.hpp"
#include <list>
#include <atomic>

class NPC : public PlayerSlot
{
	enum MoveType : int {NOTHING, START_RUNNING_LEFT, START_RUNNING_RIGHT, JUMP, SLIDE, USE_HOOK, USE_ITEM};
	struct Goal {
		sf::Vector2f position;
		float radius;

		friend bool operator== (const Goal& a, const Goal& b) {
			return a.position == b.position && a.radius == b.radius;
		}

		friend bool operator!= (const Goal& a, const Goal& b) {
			return a.position != b.position && a.radius != b.radius;
		}
	};

	const float RUN_COST = 3.0f;
	const float FREE_FALL_COST = 1.0f;
	const float HOOK_COST = 0.5f;
	const float SLIDE_COST = 3.5f;
	const float JUMP_COST_BASE_1 = 4.0f;
	const float JUMP_COST_BASE_2 = 5.0f;
	const float WALL_JUMP_COST = 1.5f;
	const float DIRECTION_CHANGE_COST = 500.0f;
	const float JUMP_COST_PER_DISTANCE_UNIT = 50.0f;

	const float THRESHOLD_PER_RADIUS_UNIT = 3.0f;

	const float CLOSENESS_THRESHOLD = 0.01f;
	const float FARNESS_THRESHOLD = 30.0f;

	using CharPtr = std::shared_ptr<Character>;
	using TilePriorityQueue = PriorityQueue<NodeData>;
	using TileNode = Node<NodeData>;
	using TileMapPtr = std::shared_ptr<TileMap>;

private:
	const int N_MOVES = 7;
	TileMapPtr tm;

	std::list<Goal> goals;
	Goal currentGoal;

	std::mutex pathMtx;
	std::mutex goalMtx;
	std::atomic<short int> pathFound = 0;
	std::atomic<bool> stopFollowing = false;
	

	TilePriorityQueue frontier;
	std::vector<TileNode> expanded;
	std::list<std::shared_ptr<TileNode> > path;

	int findExpanded(const TileNode& n) const;
	float heuristic(const TileNode& n) const;
	bool inBounds(const int i, const int j) const;
	int updateExpanded(const TileNode& current, TileNode n);
	float cost(const TileNode & current, const TileNode & next) const;
	void calculateJumpNeighbours(const TileNode& current);
	void calculateHookNeighbours(const bool right, const TileNode& current);
	void calculateWallJumpNeighbours(const bool right, TileNode& current);
	std::list<std::shared_ptr<TileNode> > buildPath(TileNode foundGoal);
	bool isGoal(const TileNode & current) const;
	bool nodeWasReached(const TileNode& n, const float closenessThreshold) const;
	bool detectDirectionChange(const TileNode& n, const TileNode& current);
	float expandToNeighbour(const TileNode & current, const int dx, const int dy);
public:
	NPC();
	TileNode getCharacterCell() const;
	void setTileMap(TileMapPtr tm);
	void addGoal(const sf::Vector2f& goalPos, const float goalRadius);
	void plan();
	void doBasicMovement(const TileNode & current, const TileNode & n, bool & jumped, bool block);
	void followPath();
	bool pathWasFound() const;
	std::list<selbaward::Line> debugLines();
	std::list<sf::RectangleShape> debugExpanded();
	std::list<sf::RectangleShape> debugHook();
};

