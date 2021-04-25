#include "PriorityQueue.h"
#include "TileMap.h"

template<class T>
bool PriorityQueue<T>::contains(const Node<T>& val) const {
	auto first = this->c.cbegin();
	auto last = this->c.cend();
	while (first != last) {
		if (sameCell(*first, val)) {
			return true;
		}
	}
	return false;
}

template <class T>
bool PriorityQueue<T>::insert(Node<T> val) {
	auto first = this->c.begin();
	auto last = this->c.end();
	while (first != last) {
		if (sameCell(*first, val)) {
			if (*first < val) first->cost = val.cost;
			return (*first < val);
		}
	}
	this->push(val);
	return true;
}

template <class T>
void PriorityQueue<T>::clear() {
	this->c.clear();
}

//Instanciation
template Node<Tiles::Collidable>;
template class PriorityQueue<Tiles::Collidable>;