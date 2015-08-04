#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Mute.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>

using namespace std;

//Menu constructor
Mute::Mute(bool m)
{
    mute = m;
}

string Mute::getText()
{
if(mute)
    return "Unmute";
else
    return "Mute";
}

void Mute::flip()
{
    mute = !mute;

}

bool Mute::getVal()
{
return mute;
}

void Mute::muteSet(sf::Sound &s)
{
    if(mute == true)
        s.setVolume(0);
    if(mute == false)
        s.setVolume(100);
}
