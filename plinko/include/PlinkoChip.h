#ifndef PLINKOCHIP_H
#define PLINKOCHIP_H

#include <SFML/Graphics.hpp>

using namespace std;

class PlinkoChip: public sf::CircleShape
{
    public:
        PlinkoChip();
    // getPosition() is already built in, don't need it

        void setVelocity(float,float);
        void continueVelocity();
        sf::Vector2f getVelocity() const;

    private:
        sf::Vector2f velocity;

};

#endif // PLINKOCHIP_H
