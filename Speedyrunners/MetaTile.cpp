#include "MetaTile.h"

void MetaTile::setType(Type type) {
	this->type = type;
}

void MetaTile::setNextNode(std::shared_ptr<Node<NodeData>> nextNode) {
	this->nextNode = nextNode;
}

MetaTile::Type MetaTile::getType() const {
	return type;
}

std::shared_ptr<Node<NodeData>> MetaTile::getNextNode() const {
	return nextNode;
}
