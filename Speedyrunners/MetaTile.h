#pragma once
#include <memory>
#include "PriorityQueue.h"
class MetaTile
{
public:
	enum Type {
		JUMP,
		HOOK,
		DIRECTION_CHANGE,
		WALL_JUMP,
		SLIDE
	};

	void setType(Type type);
	void setNextNode(std::shared_ptr<Node<NodeData> > nextNode);
	Type getType() const;
	std::shared_ptr<Node<NodeData> > getNextNode() const;
private:
	Type type;
	std::shared_ptr<Node<NodeData> > nextNode;
};

