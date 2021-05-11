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
	const float HOOK_COST = 2.5f;
	const float SLIDE_COST = 3.5f;
	const float JUMP_COST_BASE_1 = 4.0f;
	const float JUMP_COST_BASE_2 = 5.0f;
	const float WALL_JUMP_COST = 1.5f;
	const float DIRECTION_CHANGE_COST = 500.0f;
	const float JUMP_COST_PER_DISTANCE_UNIT = 50.0f;

	const float THRESHOLD_PER_RADIUS_UNIT = 3.0f;

	const sf::Time MAX_TIME_PER_STEP = sf::seconds(6.0f);
	const sf::Time GIVE_UP_TIME = sf::seconds(20.0f);

	const float CLOSENESS_THRESHOLD = 0.01f;
	const float FARNESS_THRESHOLD = 6.0f;

	using CharPtr = std::shared_ptr<Character>;
	using TilePriorityQueue = PriorityQueue<NodeData>;
	using TileNode = Node<NodeData>;
	using TileMapPtr = std::shared_ptr<TileMap>;
	using OptionalPath = std::optional< std::list<std::shared_ptr<TileNode> > >;

private:
	const int N_MOVES = 7;
	TileMapPtr tm;

	std::list<Goal> goals;
	std::shared_ptr<Goal> currentGoal[3];

	std::mutex pathMtx[2];
	std::mutex goalMtx;
	std::mutex choiceMtx;
	bool planningPath[3] = { false, false, false };
	std::atomic<short int> pathFound[3] = { 0, 0, 0};
	std::atomic<bool> stopFollowing = false;
	std::atomic<bool> active = true;
	std::atomic<bool> stitched = false;

	// 0 -> First path
	// 1 -> Next step
	// 2 -> Stitch
	TilePriorityQueue frontier[3];
	std::vector<TileNode> expanded[3];
	std::list<std::shared_ptr<TileNode> > path[3];

	int findExpanded(const TileNode& n, const int n_path) const;
	float heuristic(const TileNode& n, const Goal& goal) const;
	bool inBounds(const int i, const int j) const;
	int updateExpanded(const TileNode& current, TileNode n, const int n_path);
	float cost(const TileNode & current, const TileNode & next) const;
	void calculateJumpNeighbours(const TileNode& current, const Goal& goal, const int n_path);
	void calculateHookNeighbours(const bool right, const TileNode& current, const Goal& goal, const int n_path);
	void calculateWallJumpNeighbours(const bool right, TileNode& current, const Goal& goal, const int n_path);
	std::list<std::shared_ptr<TileNode> > buildPath(TileNode foundGoal);
	bool isGoal(const TileNode & current, const Goal& goal) const;
	void updateGoals();
	bool nodeWasReached(const TileNode& n, const float closenessThreshold) const;
	bool detectDirectionChange(const TileNode& n, const TileNode& current);
	float expandToNeighbour(const TileNode & current, const Goal& goal, const int dx, const int dy, const int n_path);
	void halt();
	void giveUp();
	float nodeDistance(const TileNode& n1, const TileNode& n2) const;
public:
	NPC();
	TileNode getCharacterCell() const;
	void setTileMap(TileMapPtr tm);
	void addGoal(const sf::Vector2f& goalPos, const float goalRadius);
	void plan();
	OptionalPath planFromTo(const int n_path, const std::shared_ptr<Goal> goal);
	bool doBasicMovement(const TileNode & current, const TileNode & n, float objDistance, bool& jumped, sf::Clock clock, bool block);
	void followPath();
	int getPathFound(int i) const;
	void endMe();
	std::list<selbaward::Line> debugLines();
	std::list<sf::RectangleShape> debugExpanded();
	std::list<sf::RectangleShape> debugHook();
};

