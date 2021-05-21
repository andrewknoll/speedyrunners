#include "Checkpoint.h"

void Checkpoint::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    // draw the vertex array
    target.draw(circle, states);
}

Checkpoint::Checkpoint(sf::Vector2f _pos, float _r) : pos(_pos), r(_r)
{
    circle.setOrigin(_r, _r);
    circle.setRadius(_r);
    circle.setPosition(_pos);
    circle.setOutlineThickness(-_r * 0.025f);
    circle.setFillColor(sf::Color::Transparent);
}

void Checkpoint::setPosition(const sf::Vector2f& _pos) {
    pos = _pos;
    circle.setPosition(_pos);
}

void Checkpoint::setRadius(const float& _r)
{
    r = _r;
    circle.setRadius(_r);
    circle.setOrigin(_r, _r);
}

sf::Vector2f Checkpoint::getPos() const
{
    return pos;
}

float Checkpoint::getRadius() const
{
    return r;
}

