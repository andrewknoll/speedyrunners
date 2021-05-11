
#pragma warning( disable : 4244 ) 
#include <cmath>
#include "utils.hpp"
#include <cmath>
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




namespace utils {

	float stopDistance(const float& speed, const float& deceleration) {
		return (speed * speed) / (2 * deceleration) - (speed * speed) / deceleration;
	}

	template <class T>
	bool contains(std::list<T> l, T v) {
		for (auto e : l) {
			if (e == v) {
				return true;
			}
		}
		return false;
	}

	int clamp(int val, int min, int max) {
		return (val < min) ? min : (val >= max) ? max : val;
	}

	float clampf(float val, float min, float max) {
		return (val < min) ? min : (val >= max) ? max : val;
	}

	float degrees(float radians) {
		return radians * 180.0 / glb::pi;
	}

	sf::Vector2f polarToCarthesian(float radius, float angle) {
		return sf::Vector2f(radius * cos(angle), radius * sin(angle));
	}

	sf::Vector2f mousePosition2f(const sf::RenderWindow& window) {
		return window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}

	float dot(const sf::Vector2f a, const sf::Vector2f b) {
		return a.x * b.x + a.y * b.y;
	}


	/*sf::Vector2i mousePosition2i(const sf::RenderWindow& window) {
		return window.mapPixelToCoords(sf::Mouse::getPosition(window));
	}*/

	// Clamps the mouse coordinates in Vector2f position to window pixel coordinates
	sf::Vector2i clampMouseCoord_old(const sf::Vector2f& pos, const sf::RenderTarget& window) {
		return sf::Vector2i(clamp(pos.x, 0, window.getSize().x-1), clamp(pos.y, 0, window.getSize().y-1));
	}

	// Clamps the mouse coordinates in Vector2f position to window pixel coordinates
	sf::Vector2i clampMouseCoord_old(const sf::RenderWindow& window) {
		sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		return clampMouseCoord(pos, window);
	}

	// Clamps the mouse coordinates in Vector2f position to window pixel coordinates
	sf::Vector2i clampMouseCoord(const sf::Vector2f& pos, const sf::RenderTarget& window) {
		return sf::Vector2i(clamp(pos.x, 0, window.getSize().x *4-1), clamp(pos.y, 0, window.getSize().y *4- 1));
	}

	// Clamps the mouse coordinates in Vector2f position to window pixel coordinates
	sf::Vector2i clampMouseCoord(const sf::RenderWindow& window) {
		sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		return clampMouseCoord(pos, window);
	}

	// https://stackoverflow.com/a/14997413/14997419
	int positiveMod(int i, int n) {
		return (i % n + n) % n;
	}

	float clampAbs(const float& val, const float& limit)
	{
		if (val > 0) {
			return (val > limit) ? limit : val;
		}
		else {
			return (val < -limit) ? -limit : val;
		}
	}

	bool inRange(float value, float min, float max) {
		return min <= value && max <= value;
	}

	sf::Vector2f clampAbs(const sf::Vector2f& val, const float& limit)
	{
		return sf::Vector2f(clampAbs(val.x, limit), clampAbs(val.y, limit));
	}

	float lerp(float x, float y, float z) {
		return ((1.0f - z) * x) + (z * y);
	}


	sf::Vector2f toVector2f(const sf::Vector2i& v) {
		return sf::Vector2f(float(v.x), float(v.y));
	}

	sf::IntRect floatToIntRect(const sf::FloatRect& fr)
	{
		return sf::IntRect(fr.left, fr.top, fr.width, fr.height);
	}

	float distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return length(p2-p1);
	}

	float length(const sf::Vector2f& v)
	{
		return sqrt(v.x*v.x+v.y*v.y);
	}

	void scaleToFullScreen(sf::Sprite& s, const float& windowHeight)
	{
		float relation = windowHeight / s.getGlobalBounds().height;
		s.setScale(relation, relation);
	}

	void scaleToFullScreen(sf::Sprite& s, const sf::RenderWindow& window)
	{
		scaleToFullScreen(s, window.getSize().y);
	}

	void scaleToFullScreen(sf::Sprite& s, const Settings& settings)
	{
		scaleToFullScreen(s, settings.windowResolution().y);
	}

	void setWidth(sf::Sprite& s, const float& width)
	{
		float rel = width / s.getGlobalBounds().width;
		s.setScale(rel, rel);
	}

	void setHeight(sf::Sprite& s, const float& h)
	{
		float rel = h / s.getGlobalBounds().height;
		s.setScale(rel, rel);
	}

	void scaleToFullScreenRatio(sf::Sprite& s, const sf::RenderWindow& window, const float& verticalRatio) {
		scaleToFullScreen(s, window.getSize().y * verticalRatio);
	}


	sf::Vector2f globalToRelative(const sf::Vector2f& global, const sf::RenderWindow& window)
	{
		return sf::Vector2f(global.x / window.getSize().x, global.y / window.getSize().y);
	}


	sf::Vector2f relativeToGlobal(sf::Vector2f relative, const sf::RenderWindow& window)
	{
		return sf::Vector2f(relative.x * window.getSize().x, relative.y * window.getSize().y);
	}

	sf::IntRect relativeToGlobalTextureRect(const sf::FloatRect& relative, const sf::Texture& tex)
	{
		auto size = tex.getSize();
		return sf::IntRect(relative.left * size.x, relative.top * size.y, relative.width*size.x, relative.height*size.y);
	}

	void scaleSpriteToTextRect(sf::Sprite& sprite)
	{
		auto& s = sprite.getScale();
		auto& rect = sprite.getTextureRect();
		sprite.setScale(s.x, s.x * (rect.height / rect.width));
	}

	void centerOrigin(sf::Sprite& s)
	{
		auto bounds = s.getLocalBounds();
		s.setOrigin(bounds.width/2.0f, bounds.height/2.0f);
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

geometry::Mat2::Mat2(const sf::Vector2f& _front, const sf::Vector2f& _up) : front(_front), up(_up)
{
}

sf::Vector2f geometry::Mat2::operator*(const sf::Vector2f& v) const
{
	return sf::Vector2f(front.x*v.x + up.x*v.y, front.y*v.x + up.y*v.y);
}

sf::Vector2f geometry::normalize(const sf::Vector2f& in)
{
	float len = utils::length(in);
	if (len != 0)  return in / len;
	else return in;
}

sf::Vector2f geometry::toWorld(const Mat2& m, const sf::Vector2f& local)
{
	return m*local;
}
