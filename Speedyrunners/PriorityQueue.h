#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include "TileMap.h"

struct NodeData {
	Tiles::Collidable tile;
	short int jumps = 2;
	bool isHooking = false;
	bool isSliding = false;
	/* -1 -> Not checked
	 *  0 -> Can't wall jump
	 *  1 -> Can wall jump
	*/
	short int canWallJumpLeft = -1;
	short int canWallJumpRight = -1;

	bool operator==(const NodeData& n2) const{
		return tile == n2.tile && jumps == n2.jumps && isHooking == n2.isHooking;
	}
	bool canJump() const {
		return jumps > 0;
	}
};

template<class T>
struct Node {
	T data;
	int cell[2];
	float heuristic;
	float cost;
	std::shared_ptr<Node<T> > prev = nullptr;


	friend bool operator< (const Node<T>& n1, const Node<T>& n2) {
		return (n1.cost + n1.heuristic) < (n2.cost + n2.heuristic);
	}

	friend bool operator> (const Node<T>& n1, const Node<T>& n2) {
		return (n1.cost + n1.heuristic) > (n2.cost + n2.heuristic);
	}

	friend bool equal(const Node<T>& n1, const Node<T>& n2) {
		return n1.cell[0] == n2.cell[0] && n1.cell[1] == n2.cell[1] && n1.data == n2.data;
	}

	friend float nodeDistance(const Node<T>& n1, const Node<T>& n2) {
		int h = n2.cell[0] - n1.cell[0];
		int v = n2.cell[0] - n1.cell[0];
		return sqrtf(h * h + v * v);
	}
	
	friend sf::Vector2f nodeCellVec(const Node<T>& n, float off_x = 0, float off_y = 0) {
		return sf::Vector2f(n.cell[0] + off_x, n.cell[1] + off_y);
	}

	friend sf::Vector2f nodeCellVec(const std::shared_ptr<Node<T> >& n, float off_x = 0, float off_y = 0) {
		return sf::Vector2f(n->cell[0] + off_x, n->cell[1] + off_y);
	}
};

template<class T>
class PriorityQueue : public std::priority_queue<Node<T>, std::vector<Node<T> >, std::greater<Node<T> > >
{
private:
	mutable std::mutex mtx;
public:
	bool safeEmpty();
	void safePush(const Node<T>& _Val);
	bool contains(const Node<T>& val) const;
	bool insert(Node<T> val);
	void clear();
	Node<T> popReturn();
};