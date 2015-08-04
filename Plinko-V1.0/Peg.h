#ifndef PEG_H
#define PEG_H

#include <SFML/Graphics.hpp>

using namespace std;

class Peg: public sf::CircleShape
{
    private:
        sf::Vector2f velocity;
        float damping; //added damping factor for pegs
        vector <sf::Color> colorVector;
        bool colorChange;

    public:
        Peg();
        Peg(bool);
        void setVelocity(float,float);
        void continueVelocity();
        sf::Vector2f getVelocity();
        float getDamping();
        void setDamping(float);
        void setRandomPosition();
        int collisionCount;
        void colorTransit();
        bool getColorChange();
};

#endif // PEG_H
