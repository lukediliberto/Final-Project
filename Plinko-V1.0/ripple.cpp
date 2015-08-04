#include <SFML/Graphics.hpp>
#include "ripple.h"

using namespace std;

Ripple::Ripple()
{
        setFillColor(sf::Color::White);
        setOutlineColor(sf::Color::Black);
        setRadius(5);
}

Ripple::Ripple(sf::Color colorTransfer, float xPos, float yPos, float initialRadius, float radialVel)
{
        sf::Color fillColor(0,0,0,0);
        setFillColor(fillColor);
        setOutlineColor(colorTransfer);
        setOutlineThickness(2);
        setRadius(initialRadius);
        setOrigin(getRadius(),getRadius());
        setPosition(xPos,yPos);
        setRadialVelocity(radialVel);

}

void Ripple::setRadialVelocity(float radialVel)
{
    radialVelocity = radialVel;
}

void Ripple::updateRipple()
{
    int fadeFactor = 2;
    sf::Color newColor(getOutlineColor().r, getOutlineColor().g, getOutlineColor().b, getOutlineColor().a/fadeFactor);
    setRadius(getRadius()+radialVelocity);
    setOrigin(getRadius(),getRadius());
    setOutlineColor(newColor);
}
