#ifndef PEG_H
#define PEG_H

#include <SFML/Graphics.hpp>

using namespace std;

class Peg: public sf::CircleShape
{
    private:
        sf::Vector2f velocity;
        float damping; //added damping factor for pegs

    public:
        Peg();
        void setVelocity(float,float);
        void continueVelocity();
        sf::Vector2f getVelocity();
        float getDamping();
        void setDamping(float);
        void setRandomPosition();
};

#endif // PEG_H
