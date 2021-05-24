#include "PriorityQueue.h"
#include "Tiles.h"


template<class T>
bool PriorityQueue<T>::safeEmpty() {
	bool res;
	mtx.lock();
	res = this->empty();
	mtx.unlock();
	return res;
}

template<class T>
void PriorityQueue<T>::safePush(const Node<T>& _Val) {
	mtx.lock();
	this->push(_Val);
	mtx.unlock();
}

template<class T>
bool PriorityQueue<T>::contains(const Node<T>& val) const {
	mtx.lock();
	auto first = this->c.cbegin();
	auto last = this->c.cend();
	while (first != last) {
		if (equal(*first, val)) {
			mtx.unlock();
			return true;
		}
		first++;
	}
	mtx.unlock();
	return false;
}

template <class T>
bool PriorityQueue<T>::insert(Node<T> val) {
	mtx.lock();
	auto first = this->c.begin();
	auto last = this->c.end();
	while (first != last) {
		if (equal(*first, val)) {
			if (*first < val) {
				first->cost = val.cost;
				first->next = val.next;
				first->prev = val.prev;
				first->data = val.data;
			}
			mtx.unlock();
			return (*first < val);
		}
		first++;
	}
	this->push(val);
	mtx.unlock();
	return true;
}

template <class T>
void PriorityQueue<T>::clear() {
	mtx.lock();
	this->c.clear();
	mtx.unlock();
}

template <class T>
Node<T> PriorityQueue<T>::popReturn() {
	mtx.lock();
	Node<T> aux = this->top();
	this->pop();
	mtx.unlock();
	return aux;
}

//Instanciation
template Node<Tiles::Collidable>;
template class PriorityQueue<Tiles::Collidable>;

template Node<NodeData>;
template class PriorityQueue<NodeData>;