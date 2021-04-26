#pragma once
#include "Character.h"
#include "Player.h"
#include "PriorityQueue.h"
#include "TileMap.h"
#include "Line.hpp"
#include <list>

class NPC : public PlayerSlot
{
	enum MoveType : int {NOTHING, START_RUNNING_LEFT, START_RUNNING_RIGHT, JUMP, SLIDE, USE_HOOK, USE_ITEM};
	/*struct Move {
		enum MoveType type;
		int heuristic;
		int cost_so_far;

		Move();
		Move(MoveType t, int h);
		bool operator< (const Move& m);
		bool operator> (const Move& m);
	};*/

	const float RUN_COST = 2.0f;
	const float FREE_FALL_COST = 1.0f;
	const float HOOK_COST = 0.5f;
	const float SLIDE_COST = 3.0f;
	const float JUMP_COST_BASE_1 = 2.5f;
	const float JUMP_COST_BASE_2 = 3.0f;
	const float JUMP_COST_PER_DISTANCE_UNIT = 10.0f;

	using CharPtr = std::shared_ptr<Character>;
	using TilePriorityQueue = PriorityQueue<NodeData>;
	using TileNode = Node<NodeData>;
	using TileMapPtr = std::shared_ptr<TileMap>;

private:
	const int N_MOVES = 7;
	TileMapPtr tm;

	TilePriorityQueue frontier;
	std::vector<TileNode> expanded;
	std::list<std::shared_ptr<TileNode> > path;

	TileNode frontierPop();
	int findExpanded(const TileNode& n) const;
	float heuristic(const TileNode& n, sf::Vector2f goalPos, float goalRadius) const;
	bool inBounds(const int i, const int j) const;
	int updateExpanded(const TileNode& current, TileNode n, float heuristic);
	float cost(const TileNode & current, const TileNode & next) const;
	void calculateJumpNeighbours(const TileNode& current, const sf::Vector2f& goalPos, const float goalRadius);
	void calculateHookNeighbours(const TileNode& current, const sf::Vector2f& goalPos, const float goalRadius);
	void buildPath(TileNode foundGoal);
	bool isGoal(const TileNode & current, const sf::Vector2f& goalPos, const float radius) const;
	float expandToNeighbour(const TileNode & current, const int dx, const int dy, const sf::Vector2f & goalPos, const float goalRadius);
public:
	NPC();
	TileNode getCharacterCell() const;
	void setTileMap(TileMapPtr tm);
	void play(const sf::Vector2f& goalPos, const float goalRadius);
	std::list<selbaward::Line> debugLines();
};

