#pragma once

#include <iostream>

#include <random>
#include <list>

#include "SFML/Graphics.hpp"
#include "Settings.h"
//#include "TileMap.h"

class RNG {
	// Para generacion de nums aleatorios, //(de https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution)

	mutable std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	mutable std::uniform_real_distribution<> dis;

public:
	RNG();

	double rand01() const; // devuelve un nº aleatorio uniforme entre 0 y 1
	double rand(const double min, const double max) const; // aleatorio uniforme entre min y max
	int rand(const int min, const int max) const; // entero uniforme en [min, max-1]
	std::string to_string() const;

	// Fills the first <size> elts of arr with random integers in [min, max)
	void fillRand(int* arr, const int size, const int min, const int max) const;

	// Vector normal aleatorio mal distribuido:
	//Vector3 vectorNormalAleatorioBruto() const;

	// Vector normal aleatorio uniforme:
	//Vector3 vectorNormalAleatorio() const;


};

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const RNG& c);

namespace utils {
	template <class T>
	bool contains(std::list<T> l, T v);

	sf::Vector2f mousePosition2f(const sf::RenderWindow& window);
	sf::Vector2i clampMouseCoord(const sf::Vector2f& pos, const sf::RenderTarget& window);

	sf::Vector2i clampMouseCoord(const sf::RenderWindow& window);

	float dot(const sf::Vector2f a, const sf::Vector2f b);

	int positiveMod(int i, int n);

	int clamp(int val, int min, int max);

	float clampf(float val, float min, float max);

	float degrees(float radians);

	sf::Vector2f polarToCarthesian(float radius, float angle);

	// Clamp + valor absoluto
	float clampAbs(const float& val, const float& limit);

	bool inRange(float value, float min, float max);

	sf::Vector2f clampAbs(const sf::Vector2f& val, const float& limit);

	sf::Vector2f toVector2f(const sf::Vector2i& v);

	sf::IntRect floatToIntRect(const sf::FloatRect& fr);

	// Distance between the points p1 and p2
	float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	// Length (modulo) of vector v
	float length(const sf::Vector2f& v);



	// Scale sprite to full window size
	void scaleToFullScreen(sf::Sprite& s, const float& windowHeight);

	// Scale sprite to full window size
	void scaleToFullScreen(sf::Sprite& s, const sf::RenderWindow& window);

	// Scale sprite to full window size
	void scaleToFullScreen(sf::Sprite& s, const Settings& settings);

	void scaleToFullScreenRatio(sf::Sprite& s, const sf::RenderWindow& window, const float& verticalRatio);


	sf::Vector2f relativeToGlobal(sf::Vector2f relative, const sf::RenderWindow& window);


	sf::IntRect relativeToGlobalTextureRect(const sf::FloatRect& relative, const sf::Texture& tex);

	// Sets the origin to the center of the sprite
	void centerOrigin(sf::Sprite& s);
}


namespace geometry {
	struct Mat2 {
		sf::Vector2f front;
		sf::Vector2f up;

		Mat2(const sf::Vector2f& _front = sf::Vector2f(1,0), const sf::Vector2f& _up = sf::Vector2f(0,1));


		sf::Vector2f operator*(const sf::Vector2f& v) const;
	};


	sf::Vector2f normalize(const sf::Vector2f& in);

	sf::Vector2f toWorld(const Mat2& m, const sf::Vector2f& local);
}

template <typename T>
std::string to_string(const sf::Vector2<T>& v);
template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v);



namespace physics {
	const float GRAVITY = 25*9.821;
	const float MAX_FALL_SPEED = 300;
	const float FLOOR_FRICTION = GRAVITY*1.5;
	const float AIR_FRICTION = GRAVITY * 0.5;

}

template<typename T>
inline std::string to_string(const sf::Vector2<T>& v)
{
	return std::to_string(v.x) + " " + std::to_string(v.y);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v)
{
	return os << to_string(v);
}
