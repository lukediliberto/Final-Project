#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "PlinkoChip.h"
#include "Peg.h"
#include "MomentumTransfer.h"
#include "Menu.h"
#include "Game.h"
#include "Mute.h"
using namespace std;


//Notes for running this program:
    //Make sure that this program is created in an SFML project.
    //You need to add the "sfml-audio" library.
    //^^To do this: Project -> Build Options -> Linker Settings -> Add -> "sfml-audio" -> Ok

int main()
{
    //srand for random # generation should only be declared once per main execution
    srand(time(NULL));
    int window_width = 500;
    int window_height = 500;
    Mute mute(0); /*Initialize mute off*/
    //open the main menu
    menufunc(window_width, window_height, mute);
    return EXIT_SUCCESS;
}

