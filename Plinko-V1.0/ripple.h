#ifndef RIPPLE_H
#define RIPPLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Peg.h"
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Ripple: public sf::CircleShape
{
    private:
        float radialVelocity;

    public:
        Ripple();
        Ripple(sf::Color, float, float, float, float);

        void setRadialVelocity(float);
        float getRadialVelocity();
        void updateRipple();

        void setIsColliding(bool);
        bool getIsColliding() const;
};

#endif // RIPPLE_H
