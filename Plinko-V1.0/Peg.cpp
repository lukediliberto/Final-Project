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
    collisionCount = 0;
    colorChange = false;
}
Peg::Peg(bool colorChange)
{
    setRadius(5);
    setFillColor(sf::Color::Black);
    setOrigin(sf::Vector2f(5,5));
    setPointCount(100);
    velocity.x=0;
    velocity.y=0;
    collisionCount = 0;
    colorChange;
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

void Peg::colorTransit()
{
    if(colorChange)
    {
        if(collisionCount%5==0)
            setFillColor(sf::Color::Magenta);
        else if (collisionCount%5==1)
            setFillColor(sf::Color::Red);
        else if (collisionCount%5==2)
            setFillColor(sf::Color::Green);
        else if (collisionCount%5==3)
            setFillColor(sf::Color::Blue);
        else
            setFillColor(sf::Color::Cyan);
    }
}

bool Peg::getColorChange()
    {return colorChange;}
