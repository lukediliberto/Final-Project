#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "PlinkoChip.h"
#include "Peg.h"
#include "MomentumTransfer.h"
#include "Menu.h"
#include "Game.h"

using namespace std;

int main()
{
    //srand for random # generation should only be declared once per main execution
    srand(time(NULL));
    int window_width = 500;
    int window_height = 500;
    menufunc(window_width, window_height);

    //Note:
    //Sometimes the chip appears to jump between bins at the very end.
    //Need to add statements testing whether text and images are present.


    return EXIT_SUCCESS;
}

