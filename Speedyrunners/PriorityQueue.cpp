#include "PriorityQueue.h"
#include "TileMap.h"

template<class T>
bool PriorityQueue<T>::contains(const Node<T>& val) const {
	auto first = this->c.cbegin();
	auto last = this->c.cend();
	while (first != last) {
		if (equal(*first, val)) {
			return true;
		}
		first++;
	}
	return false;
}

template <class T>
bool PriorityQueue<T>::insert(Node<T> val) {
	auto first = this->c.begin();
	auto last = this->c.end();
	while (first != last) {
		if (equal(*first, val)) {
			if (*first < val) {
				first->cost = val.cost;
				first->prev = val.prev;
				first->data = val.data;
			}
			return (*first < val);
		}
		first++;
	}
	this->push(val);
	return true;
}

template <class T>
void PriorityQueue<T>::clear() {
	this->c.clear();
}

template <class T>
Node<T> PriorityQueue<T>::popReturn() {
	Node<T> aux = this->top();
	this->pop();
	return aux;
}

//Instanciation
template Node<Tiles::Collidable>;
template class PriorityQueue<Tiles::Collidable>;

template Node<NodeData>;
template class PriorityQueue<NodeData>;