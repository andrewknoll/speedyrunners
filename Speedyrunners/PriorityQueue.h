#pragma once
#include <queue>
#include <memory>
#include "TileMap.h"

struct NodeData {
	Tiles::Collidable tile;
	bool canJump = true;
	bool isHooking = false;
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

	friend bool sameCell(const Node<T>& n1, const Node<T>& n2) {
		return n1.cell[0] == n2.cell[0] && n1.cell[1] == n2.cell[1];
	}
	
};

template<class T>
class PriorityQueue : public std::priority_queue<Node<T>, std::vector<Node<T> >, std::greater<Node<T> > >
{
public:
	bool contains(const Node<T>& val) const;
	bool insert(Node<T> val);
	void clear();
};