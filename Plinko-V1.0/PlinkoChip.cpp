#include <SFML/Graphics.hpp>
#include "PlinkoChip.h"

using namespace std;

PlinkoChip::PlinkoChip()
{
    setRadius(15);
    //setFillColor(sf::Color::Black);
    setOrigin(sf::Vector2f(15,15));
    setPointCount(100);
    velocity.x=0;
    velocity.y=0;
    isColliding=0;
}

PlinkoChip::PlinkoChip(float xVelocity, float yVelocity, float xInitPos,
                        float yInitPos, bool wb, int ww, int wh)
{
        //sf::Color ChipDefault(160,160,160);
        setRadius(15);
        //setFillColor(ChipDefault);
        setOrigin(getRadius(),getRadius());
        setPosition(xInitPos,yInitPos);
        setVelocity(xVelocity,yVelocity);
        setWindowWidth(ww);
        setWindowHeight(wh);
        setWindowBound(wb);
        setNextPosition();
        setMass(5);
        isColliding=0;
}

void PlinkoChip::setVelocity(float xSpeed,float ySpeed)
{
    velocity.x=xSpeed;
    velocity.y=ySpeed;
//    move(xspeed,yspeed);
}

void PlinkoChip::setVelocity(sf::Vector2f vec)
{
    velocity.x = vec.x;
    velocity.y = vec.y;
}

void PlinkoChip::continueVelocity()
{
    setVelocity(velocity.x,velocity.y);
}

sf::Vector2f PlinkoChip::getVelocity()
{
    return velocity;
}

void PlinkoChip::setWindowWidth(int ww)
{
    windowWidth = ww;
}

void PlinkoChip::setWindowHeight(int wh)
{
    windowHeight = wh;
}

void PlinkoChip::setWindowBound(bool wb)
{
    windowBound = wb;
}

bool PlinkoChip::getWindowBound()
{
    return windowBound;
}

float PlinkoChip::getMass()
{
    return mass;
}

void PlinkoChip::setMass(float m)
{
    mass = m;
}

void PlinkoChip::applyGravity()
{
    setVelocity(getVelocity().x,getVelocity().y+.3);
}

void PlinkoChip::setNextPosition()
{
    nextPosition.x = getPosition().x + velocity.x;
    nextPosition.y = getPosition().y + velocity.y;

    if(getWindowBound())
    {
        if (nextPosition.x - getRadius() < 0) {
                velocity.x*= -1;
                nextPosition.x = 0 + getRadius();
            } else if (nextPosition.x + getRadius()>= windowWidth) {
                velocity.x*= -1;
                nextPosition.x = windowWidth - getRadius();
            } else if (nextPosition.y - getRadius()< 0) {
                velocity.y*= -1;
                nextPosition.y = 0 + getRadius();
            } else if (nextPosition.y + getRadius()>= windowHeight) {
                velocity.y*= -1;
                nextPosition.y = windowHeight - getRadius();
            }
    }
}

sf::Vector2f PlinkoChip::getNextPosition()
{
    return nextPosition;
}

void PlinkoChip::setIsColliding(bool value)
{
    isColliding=value;
}

bool PlinkoChip::getIsColliding() const
{
    return isColliding;
}

