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
}

sf::Vector2f Checkpoint::getPos() const
{
    return pos;
}

float Checkpoint::getRadius() const
{
    return r;
}

