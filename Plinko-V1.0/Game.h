#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Peg.h"
#include <iostream>
#include <string>

using namespace std;

void checkForUniquePosition (vector<Peg> &pegVec);
void gamefunc(char c);


#endif // GAME_H
