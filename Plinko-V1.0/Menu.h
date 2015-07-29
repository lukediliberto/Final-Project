#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Peg.h"
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

void menufunc(float width, float height);
void subfunc(float width, float height, sf::RenderWindow &submenuwindow, Menu menu);
void pausefunc(float width, float height, sf::RenderWindow &window);
void winfunc(float width, float height, sf::RenderWindow &winwindow, char c, int cash, vector<Peg> allPegs);

#endif // MENU_H
