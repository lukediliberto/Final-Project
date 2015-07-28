#include <SFML/Graphics.hpp>
#include "Peg.h"

using namespace std;

Peg::Peg()
{
    setRadius(5);
    setFillColor(sf::Color::Black);
    setOrigin(sf::Vector2f(5,5));
    setPointCount(100);
    velocity.x=0;
    velocity.y=0;

}
//overload constructor for inline position setting and peg v bin colors etc.
Peg::Peg(float xPos, float yPos, float rad, uint8_t r, uint8_t g, uint8_t b, float d)
{
    setRadius(rad);
    sf::Color pegColor(r,g,b);
    setFillColor(pegColor);
    setOrigin(sf::Vector2f(5,5));
    setPointCount(100);
    setPosition(xPos,yPos);
    velocity.x=0;
    velocity.y=0;
    //setDamping(d);
}

void Peg::setVelocity(float xspeed,float yspeed)
{
    velocity.x=xspeed;
    velocity.y=yspeed;
    move(xspeed,yspeed);
}

void Peg::continueVelocity()
{
    setVelocity(velocity.x,velocity.y);
}

sf::Vector2f Peg::getVelocity()
{
    return velocity;
}

float Peg::getDamping()
{
    return damping;
}

void Peg::setDamping(float d)
{
    damping=d;
}

void Peg::setRandomPosition()
{
    //generating random numbers between stoneRadius +1 and about (500-stoneRadius-1)
    //shrunk the starting position by 1 pixel on all sides
    //so that there would be less potential glitches
    float xposition=rand() % 500;
    float yposition=rand() % 350 + 50;

    setPosition(xposition,yposition);
}

