#include <SFML/Graphics.hpp>
#include "PlinkoChip.h"

using namespace std;

PlinkoChip::PlinkoChip()
{
    setRadius(15);
    setFillColor(sf::Color::Black);
    setOrigin(sf::Vector2f(15,15));
    setPointCount(100);
    velocity.x=0;
    velocity.y=0;
}

void PlinkoChip::setVelocity(float xspeed,float yspeed)
{
    velocity.x=xspeed;
    velocity.y=yspeed;
    move(xspeed,yspeed);
}

void PlinkoChip::continueVelocity()
{
    setVelocity(velocity.x,velocity.y);
}


sf::Vector2f PlinkoChip::getVelocity() const
{
    return velocity;
}

