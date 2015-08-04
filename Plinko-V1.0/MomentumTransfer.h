#ifndef MOMENTUMTRANSFER_H
#define MOMENTUMTRANSFER_H

#include <SFML/Graphics.hpp>
#include "Peg.h"
#include "PlinkoChip.h"
#include "ripple.h"

using namespace std;

void MomentumTransfer(PlinkoChip&, Peg&);
float get2DMagnitude(sf::Vector2f);
float get2DDotProduct(sf::Vector2f,sf::Vector2f);
float getDistance(sf::Vector2f,sf::Vector2f);


#endif // MOMENTUMTRANSFER_H
