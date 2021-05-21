
#pragma warning( disable : 4244 ) 
#include <sstream>
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
	if (int(r)==max && min!=max) return int(r)-1; // que no devuelva max
	return int(r);
}

void RNG::fillRand(int* arr, const int size, const int min, const int max) const
{
	for (int i = 0; i < size; i++) {
		arr[i] = rand(min, max);
	}
}




namespace utils {

	float distanceToLine(sf::Vector2f l0, sf::Vector2f lf, sf::Vector2f p) {
		return std::abs((lf.x - l0.x) * (l0.y - p.y) - (l0.x - p.x) * (lf.y - l0.y)) / sqrtf((lf.x - l0.x) * (lf.x - l0.x) + (lf.y - l0.y) * (lf.y - l0.y));
	}

	float parseFloat(std::istringstream& is, const char sep) {
		std::string token;
		if (!getline(is, token, sep)) return 0;
		return stof(token);
	}

	int parseInt(std::istringstream& is, const char sep) {
		std::string token;
		if (!getline(is, token, sep)) return 0;
		return stoi(token);
	}

	sf::Color parseColor(std::istringstream& is, const char sep)
	{
		std::vector<int>rgb;
		for (int i = 0; i < 3; i++) rgb.push_back(parseInt(is, sep));
		return sf::Color(rgb[0], rgb[1], rgb[2]);
	}

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

	template<typename T>
	sf::Vector2i toVector2i(const sf::Vector2<T>& v)
	{
		return sf::Vector2i(v.x,v.y);
	}


	template<typename T>
	sf::Vector2u toVector2u(const sf::Vector2<T>& v)
	{
		return sf::Vector2u(v.x,v.y);
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
		return min <= value && value <= max;
	}

	sf::Vector2f clampAbs(const sf::Vector2f& val, const float& limit)
	{
		return sf::Vector2f(clampAbs(val.x, limit), clampAbs(val.y, limit));
	}

	float lerp(float x, float y, float z) {
		return ((1.0f - z) * x) + (z * y);
	}

	sf::Color lerp(sf::Color x, sf::Color y, float z)
	{
		return sf::Color(lerp(x.r,y.r,z), lerp(x.g,y.g,z), lerp(x.b,y.b,z),lerp(x.a,y.a,z));
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

	sf::Vector2i spritesForFloatRect(const sf::FloatRect& rect)
	{
		return sf::Vector2i(1.0f / rect.width, 1.0f/rect.height);
	}

	void scaleToFullScreen(sf::Sprite& s, const float& windowHeight)
	{
		float relation = windowHeight / s.getGlobalBounds().height;
		s.setScale(relation, relation);
	}

	void scaleToFullScreen(sf::Sprite& s, const sf::RenderWindow& window)
	{
		scaleToFullScreen(s, window.getView().getSize().y);
	}

	void scaleToFullScreen(sf::Sprite& s, const Settings& settings)
	{
		scaleToFullScreen(s, settings.windowResolution().y);
	}

	sf::IntRect texRectInGrid(const sf::Texture& tex, int nX, int nY, int idxX, int idxY)
	{
		auto size2u = tex.getSize(); // tex size
		auto size = sf::Vector2i(size2u.x / nX, size2u.y / nY); // Rect size
		return sf::IntRect(idxX * size.x, idxY * size.y, size.x, size.y);
	}

	sf::IntRect texRectInGrid(const sf::Texture& tex, const sf::Vector2i& nSprites, const sf::Vector2i& idx)
	{
		return texRectInGrid(tex, nSprites.x, nSprites.y, idx.x, idx.y);
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

sf::Transform geometry::Mat2::toTransform() const
{
	return sf::Transform(front.x, front.y, 0, up.x, up.y, 0, 0,0,1);
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



sf::Vector2f geometry::reduceComponent(sf::Vector2f vel, const sf::Vector2f& up, const float& horizontalDrag)
{

	return sf::Vector2f(vel.x * horizontalDrag * up.x, vel.y * horizontalDrag * up.y);
}

rng::IntSampler::IntSampler(const int min, const int max) : dis(min, max)
{
}

int rng::IntSampler::sample() const
{
	return dis(gen);
}
