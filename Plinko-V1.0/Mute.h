#ifndef MUTE_H
#define MUTE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Peg.h"
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Mute
{
    private:
        bool mute;
        string muteText;
        vector <sf::Sound*> soundVector;

    public:
        Mute(bool);
        string getText();
        void flip();
        void muteSound(sf::Sound &s);
        bool getVal();
        void addSound(sf::Sound &sound);
        void muteSet(sf::Sound &s);
};


#endif // MUTE_H

