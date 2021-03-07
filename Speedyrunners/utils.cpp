#include <cmath>
#include "utils.hpp"
#include "SFML/Graphics.hpp"


std::random_device rd;  //Will be used to obtain a seed for the random number engine

// inicializa el generador de nºs aleatorios
RNG::RNG() : gen(rd()), dis(0.0,1.0)
{}

// devuelve un nº aleatorio uniforme entre 0 y 1
double RNG::rand01() const {
	return dis(gen);
}

// devuelve un nº aleatorio uniforme entre min y max
double RNG::rand(const double min, const double max) const {
	double r = rand01();
	return r*(max-min)+min;
	//return dis(gen);
}

// Devuelve un entero aleatorio entre [min..max), todos aprox. equiprobables
int RNG::rand(const int min, const int max) const {
	double r = rand(double(min), double(max+1));
	if (int(r)==max) return int(r)-1; // que no devuelva max
	return int(r);
}

void RNG::fillRand(int* arr, const int size, const int min, const int max) const
{
	for (int i = 0; i < size; i++) {
		arr[i] = rand(min, max);
	}
}

int clamp(int val, int min, int max) {
	return (val < min) ? min : (val >= max) ? max : val;
}


namespace utils {
	// Clamps the mouse coordinates in Vector2f position to window pixel coordinates
	sf::Vector2i clampMouseCoord(const sf::Vector2f& pos, const sf::RenderTarget& window) {
		return sf::Vector2i(clamp(pos.x, 0, window.getSize().x-1), clamp(pos.y, 0, window.getSize().y-1));
	}
}



// Vector3 RNG::vectorNormalAleatorio() const {
// 	double rand1 = gen.rand01();
// 	double rand2 = gen.rand01();
// 	double theta = acos(sqrt(1.0-rand01()));
// 	double phi = 2.0 * PI * rand2;
// 	// std::cout << "---------------------------------" << '\n';
// 	// std::cout << "theta: " << theta << "\nphi: " <<phi << '\n';
// 	// std::cout << "base: " << base << '\n';
// 	wi = base * Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta), false);
// 	return normalizar(Vector3(rand01()-0.5, rand01()-0.5, rand01()-0.5, false));
// }


