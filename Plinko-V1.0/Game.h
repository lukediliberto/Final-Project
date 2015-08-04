#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Peg.h"
#include "Mute.h"
#include "Menu.h"
#include <iostream>
#include <string>

using namespace std;

void checkForUniquePosition (vector<Peg> &pegVec);
void gamefunc(char c, Mute &mute);


#endif // GAME_H
