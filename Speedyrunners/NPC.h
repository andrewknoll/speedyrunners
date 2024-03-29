#pragma once
#include "Character.h"
#include "Player.h"
#include "PriorityQueue.h"
#include "TileMap.h"
#include "Line.hpp"
#include "Checkpoint.h"
#include <deque>
#include <atomic>

//#define DEBUG_PATH

class NPC : public PlayerSlot
{
	using CharPtr = std::shared_ptr<Character>;
	using TilePriorityQueue = PriorityQueue<NodeData>;
	using TileNode = Node<NodeData>;
	using TileMapPtr = std::shared_ptr<TileMap>;
	using OptionalPath = std::optional< std::deque<std::shared_ptr<TileNode> > >;
	using PathIterator = std::deque<std::shared_ptr<TileNode> >::const_iterator;

	enum MoveType : int {NOTHING, START_RUNNING_LEFT, START_RUNNING_RIGHT, JUMP, SLIDE, USE_HOOK, USE_ITEM};
	struct Goal {
		sf::Vector2f position;
		float radius;

		Goal(const sf::Vector2f& position, const float radius) :
			position(position),
			radius(radius)
		{}

		Goal(const Checkpoint& c) :
			position(c.getPos()),
			radius(c.getRadius())
		{}

		Goal(const TileNode& tn, int r) :
			position(sf::Vector2f(tn.cell[0], tn.cell[1])),
			radius(r)
		{}

		friend bool operator== (const Goal& a, const Goal& b) {
			return a.position == b.position && a.radius == b.radius;
		}

		friend bool operator!= (const Goal& a, const Goal& b) {
			return a.position != b.position && a.radius != b.radius;
		}

	};

	const float RUN_COST = 1.0f;
	const float FREE_FALL_COST = 1.5f;
	const float HOOK_COST = 5.0f;
	const float SLIDE_COST = 6.0f;
	const float JUMP_COST_BASE_1 = 2.0f;
	const float JUMP_COST_BASE_2 = 4.0f;
	const float WALL_JUMP_COST = 1.5f;
	const float DIRECTION_CHANGE_COST = 3.0f;
	const float JUMP_COST_PER_DISTANCE_UNIT = 100.0f;

	const float THRESHOLD_PER_RADIUS_UNIT = 5.0f;

	const sf::Time MAX_TIME_PER_STEP = sf::seconds(6.0f);
	const sf::Time GIVE_UP_TIME = sf::seconds(2.0f);

	const float CLOSENESS_THRESHOLD = 1.0f;
	const float FARNESS_THRESHOLD = 10.0f;
	const float USE_ITEM_THRESHOLD = 100.0f;

private:
	const int N_MOVES = 7;
	TileMapPtr tm;

	std::vector<Goal> goals;
	int currentGoalIdx;
	bool tryingToFindAir = false;
	bool mustReposition = false;
	bool usedHook = false;

	std::mutex pathMtx[2];
	std::mutex goalMtx;
	std::mutex choiceMtx;
	bool planningPath[3] = { false, false, false };
	std::atomic<short int> pathFound[3] = { 0, 0, 0}; // -1 not searched, 0 not found, 1 found
	std::atomic<bool> stopFollowing = false;
	std::atomic<bool> active = true;
	std::atomic<bool> resetPlan = false;
	std::atomic<bool> stitched = false;
	std::atomic<bool> useItem = false;

	// 0 -> First path
	// 1 -> Next step
	// 2 -> Stitch
	TilePriorityQueue frontier[3];
	std::vector<TileNode> expanded[3];
	std::deque<std::shared_ptr<TileNode> > path[3];

	// Path following experiment:
	sf::Time elapsed;
	sf::Vector2f position;
	bool jumped = false, wallJumped = false;
	TileNode current;//
	PathIterator step;
	int retryCount = 0;

	std::shared_ptr<MetaTile> currentMT;
	sf::Vector2i MTposition;

	bool isPerformingWallJump = false;
	bool wallJumpStep2 = false;
	sf::Vector2f distHook;
	sf::Vector2f hookPrevPos;
	sf::Vector2f accDistHook;
	

	int findExpanded(const TileNode& n, const int n_path) const;
	float heuristic(const TileNode& n, const Goal& goal) const;
	bool inBounds(const int i, const int j) const;
	int updateExpanded(const TileNode& current, TileNode n, const int n_path);
	float cost(const TileNode & current, const TileNode & next) const;
	void calculateJumpNeighbours(const TileNode& current, const Goal& goal, const int n_path);
	void calculateHookNeighbours(const bool right, const TileNode& current, const Goal& goal, const int n_path);
	void calculateWallJumpNeighbours(const bool right, TileNode& current, const Goal& goal, const int n_path);
	void buildPath(TileNode foundGoal, std::deque<std::shared_ptr<NPC::TileNode>>& newPath);
	bool isGoal(const TileNode & current, const Goal& goal) const;
	bool isGoal(const TileNode & current, const TileNode& goal, int radius) const;
	//void updateGoals();
	bool nodeWasReached(const TileNode& n, const float closenessThreshold) const;
	bool detectDirectionChange(const TileNode& n, const TileNode& current);
	float expandToNeighbour(const TileNode & current, const Goal& goal, const int dx, const int dy, const int n_path);
	void halt();
	void giveUp();
	float nodeDistance(const TileNode& n1, const TileNode& n2) const;
	PathIterator getClosestNode(const TileNode& current, const std::deque<std::shared_ptr<TileNode> >& p) const;
	void moveWithoutPath(); // tries to get closer to the objective before having the path
	void tryToWallJump();
	bool detectWallJump(bool right, float widthMultiplier);// returns if there are jump wall to the <right>, in a rectangle of width hitbox*widthMultiplier
public:
	NPC();
	TileNode getCharacterCell() const;
	void setTileMap(TileMapPtr tm);
	void setCheckpoints(const std::vector<Checkpoint>& cp);
	void setActiveCheckpoint(const int c);
	//void addGoal(const sf::Vector2f& goalPos, const float goalRadius);
	void plan();
	void replan();
	void planFromTo(const int n_path, const std::shared_ptr<Goal> origin, const std::shared_ptr<Goal> goal, OptionalPath& newPath);
	void doBasicMovement(const TileNode& current, const TileNode& n, bool block);
	//void followPath();
	void update(const sf::Time dT);
	int getPathFound(int i) const;
	void endMe();
	void clearPaths();
	bool getAndResetUseItem();

	void die(); // resets states of path follower part
#ifdef DEBUG_PATH
	std::list<selbaward::Line> debugLines();
	std::list<sf::RectangleShape> debugExpanded();
	std::list<sf::RectangleShape> debugHook();
	sf::RectangleShape debugCurrentPos();
#endif
};
