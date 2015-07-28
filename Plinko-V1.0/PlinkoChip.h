#ifndef PLINKOCHIP_H
#define PLINKOCHIP_H

#include <SFML/Graphics.hpp>

using namespace std;

class PlinkoChip: public sf::CircleShape
{
    private:
        sf::Vector2f velocity;
        sf::Vector2f nextPosition;
        float mass;
        bool windowBound;
        int windowWidth, windowHeight;

        bool isColliding;

    public:
        PlinkoChip();
        PlinkoChip(float,float,float,float,bool,int,int);

        void setVelocity(float,float);
        void setVelocity(sf::Vector2f);
        void continueVelocity();
        sf::Vector2f getVelocity();
        void setNextPosition();
        sf::Vector2f getNextPosition();
        void applyGravity();

        void setWindowWidth(int);
        void setWindowHeight(int);
        void setWindowBound(bool);
        bool getWindowBound();

        float getMass();
        void setMass(float);

        void setIsColliding(bool);
        bool getIsColliding() const;
};

#endif // PLINKOCHIP_H
