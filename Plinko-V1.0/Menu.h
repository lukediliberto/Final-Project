#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Peg.h"
#include "PlinkoChip.h"
class Mute;
#include <iostream>
#include <string>
#include <vector>

//#define NUM_ITEMS 3

using namespace std;

class Menu
{
    private:
    int selectedIndex;
    sf::Font font;
    sf::Text menuText[10];
    int NUM_ITEMS;
    sf::Color textcolor;

    public:
        Menu(float width, float height, int num, sf::Color, unsigned int);
        void draw(sf::RenderWindow &window);
        void MoveUp();
        void MoveDown();
        int GetSelected();
        void SetMenuItems(int index, string text);
        void SetNumItems(int num);
        int GetNumItems();
        void SetTextPosition(int index, float x, float y);
        void SetTextColor(sf::Color);

};

void menufunc(float width, float height, Mute &mute);
void subfunc(float width, float height, Mute &mute, sf::RenderWindow &submenuwindow, Menu menu);
void pausefunc(float width, float height, Mute &mute, sf::RenderWindow &window, sf::Sound &gamemusic, vector<Peg> allPegs,
                PlinkoChip testChip, sf::Text binMoney[], sf::Sprite scoreboard, sf::Sprite rightPlinkoDoor,sf::Sprite brita,
                sf::Text chipText, sf::Text moneyText);
void winfunc(float width, float height, Mute &mute, sf::RenderWindow &winwindow, char c, int cash, vector<Peg> allPegs);


#endif // MENU_H
