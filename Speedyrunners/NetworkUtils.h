#pragma once
#include "SFML/Network.hpp"
#include <memory>
#include <vector>
#include <list>
#include "Item.h"
#include "Character.h"
#include "NPC.h"
#include "RoundVictory.h"
#include "Camera.h"
#include "Level.h"
#include "Countdown.h"
#include <tuple>
/*
template <typename T, typename S>
sf::Packet operator<< (const sf::Packet& p, S obj) {
	if (std::is_same < S, std::shared_ptr<T>::value) {
		return sharedPtrOverloadIn<T>(p, obj);
	}
	else if (std::is_same < S, std::vector<T>::value) {
		return vectorOverloadIn<T>(p, obj);
	}
	else if (std::is_same < S, std::list<T>::value) {
		return listOverloadIn<T>(p, obj);
	}
	else {
		return p << obj;
	}
}

template <typename T, typename S>
sf::Packet operator>> (sf::Packet& p, S obj) {
	if (std::is_same < S, std::shared_ptr<T>::value) {
		return sharedPtrOverloadOut<T>(p, obj);
	}
	else if (std::is_same < S, std::vector<T>::value) {
		return vectorOverloadOut<T>(p, obj);
	}
	else if (std::is_same < S, std::list<T>::value) {
		return listOverloadOut<T>(p, obj);
	}
	else {
		return p >> obj;
	}
}
*/

template <typename T>
sf::Packet& sharedPtrOverloadIn(sf::Packet& p, const std::shared_ptr<T> obj) {
	p.append((void*)obj.get(), sizeof(*obj));
	return p;
}

template <typename T>
sf::Packet& sharedPtrOverloadOut(sf::Packet& p, std::shared_ptr<T> obj) {
	obj = std::shared_ptr<T>((T*)p.getData());
	return p;
}

template <typename T>
sf::Packet& vectorOverloadIn(sf::Packet& p, const std::vector<T> obj) {
	p << obj.size();
	for (T t : obj) {
		void* ptr = &t;
		p.append(ptr, sizeof(t));
	}
	return p;
}

template <typename T>
sf::Packet& vectorOverloadOut(sf::Packet& p, std::vector<T>& obj) {
	sf::Uint32 size;
	auto content = p >> size;
	T* buffer = (T*)content.getData();
	obj = std::vector<T>(buffer, buffer + size);
	return p;
}

template <typename T>
sf::Packet& listOverloadIn(sf::Packet& p, const std::list<T> obj) {
	p << obj.size();
	for (T t : obj) {
		void* ptr = &t;
		p.append(ptr, sizeof(t));
	}
	return p;
}

template <typename T>
sf::Packet& listOverloadOut(sf::Packet& p, std::list<T>& obj) {
	sf::Uint32 size;
	auto content = p >> size;
	T* buffer = (T*)content.getData();
	obj = std::list<T>(buffer, buffer + size);
	return p;
}


//Template Instanciation

template<typename... Ts>
auto instantiateSPtr() {
	static auto funcs = std::tuple_cat(std::make_tuple(sharedPtrOverloadIn<Ts>, sharedPtrOverloadOut<Ts>)...);

	return &funcs;
}

template<typename... Ts>
auto instantiateV() {
	static auto funcs = std::tuple_cat(std::make_tuple(vectorOverloadIn<Ts>, vectorOverloadOut<Ts>)...);

	return &funcs;
}

template<typename... Ts>
auto instantiateL() {
	static auto funcs = std::tuple_cat(std::make_tuple(listOverloadIn<Ts>, listOverloadOut<Ts>)...);

	return &funcs;
}




template auto instantiateSPtr <RoundVictory, Countdown, Camera, Level>();


template auto instantiateV < particles::PSystem, std::shared_ptr<Item>, Checkpoint, std::shared_ptr<NPC> >();


template auto instantiateL <std::shared_ptr<Character> >();