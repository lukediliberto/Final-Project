#include <SFML/Graphics.hpp>
#include "MomentumTransfer.h"

using namespace std;

float get2DMagnitude(sf::Vector2f vec)
{
    return sqrt(pow(vec.x,2)+pow(vec.y,2));
}

float get2DDotProduct(sf::Vector2f vec1,sf::Vector2f vec2)
{
    return vec1.x*vec2.x+vec1.y*vec2.y;
}

float getDistance(sf::Vector2f vec1,sf::Vector2f vec2)
{
    return sqrt(pow(vec1.x-vec2.x,2)+pow(vec1.y-vec2.y,2));
}

void MomentumTransfer(PlinkoChip& c1, Peg& p1)
{
		sf::Vector2f normalVector, unitNormalVector, unitTangentVector, v1normnew, v2normnew, v1tannew, v2tannew;
		float v1tan, v1norm, v2tan, v2norm;

        sf::Vector2f position1=c1.getPosition();
        sf::Vector2f position2=p1.getPosition();

        float xdistanceSquared=(position2.x-position1.x)*(position2.x-position1.x);
        float ydistanceSquared=(position2.y-position1.y)*(position2.y-position1.y);

        float distance=sqrt(xdistanceSquared+ydistanceSquared);

        double collisionDistance=c1.getRadius()+p1.getRadius();


        if (distance<=collisionDistance && !c1.getIsColliding())
        {
            //Physics for collisions
            normalVector = sf::Vector2f(p1.getPosition().x-c1.getPosition().x,p1.getPosition().y-c1.getPosition().y);
            unitNormalVector = sf::Vector2f(normalVector.x/get2DMagnitude(normalVector),normalVector.y/get2DMagnitude(normalVector));
            unitTangentVector = sf::Vector2f((-1)*unitNormalVector.y,unitNormalVector.x);
            v1tan = get2DDotProduct(unitTangentVector,c1.getVelocity());
            v1norm = get2DDotProduct(unitNormalVector,c1.getVelocity());
            v2tan = get2DDotProduct(unitTangentVector,p1.getVelocity());
            v2norm = get2DDotProduct(unitNormalVector,p1.getVelocity());
            //Post Collision Variables
            v1tannew = v1tan*unitTangentVector;
            v2tannew = v2tan*unitTangentVector;
            v1normnew = sf::Vector2f((-v1norm+2*v2norm)*unitNormalVector.x,
                                    (-v1norm+2*v2norm)*unitNormalVector.y);
            sf::Vector2f c1vNew = sf::Vector2f(v1tannew+v1normnew);
            //adding damping factor:
            //c1vNew.x = c1vNew.x/p1.getDamping();
            //c1vNew.y = c1vNew.y/p1.getDamping();

            //Note: this appears to work well!
            //damping factor (acts like gravity)
            c1vNew.x=c1vNew.x/1.5;
            c1vNew.y=c1vNew.y/1.5;


            c1.setVelocity(c1vNew);

            c1.setIsColliding(1);

            //handling case where chip bounces up and down on a single peg
            //impart a small (.01) positive or negative x velocity
            if(c1.getVelocity().x == 0 && normalVector.x == 0)
                {
                int r1 = rand()%2;
                if(r1%2 == 0)
                    {c1.setVelocity(.01,c1.getVelocity().y);}
                else
                    {c1.setVelocity(-.01,c1.getVelocity().y);}
                }
        }

    //this prevents the stones from "swirling" off each other
        while (c1.getIsColliding())
        {
            //update position of Chip
            c1.setNextPosition();
            c1.setPosition(c1.getNextPosition().x,c1.getNextPosition().y);
            c1.applyGravity();
            //^^This is the same thing as in main^^

            sf::Vector2f position1=c1.getPosition();
            sf::Vector2f position2=p1.getPosition();

            float xdistanceSquared=(position2.x-position1.x)*(position2.x-position1.x);
            float ydistanceSquared=(position2.y-position1.y)*(position2.y-position1.y);

            float distance=sqrt(xdistanceSquared+ydistanceSquared);

            if (distance>collisionDistance)
            {
                c1.setIsColliding(0);
            }
        }

}
