#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int FRAME_RATE = 10;

Menu::Menu(float width, float height, int num)
{
    NUM_ITEMS = num;

    if(!font.loadFromFile("PLINKO2K.TTF"))
        cout<<"Error loading Font (Menu Constructor)"<<endl;

    for(int i = 0; i<NUM_ITEMS; i++)
    {
        menuText[i].setFont(font);
        menuText[i].setColor(sf::Color::Black);
        menuText[i].setPosition(sf::Vector2f(width/30, height-150 + 30*i));
        if(i == 0)
            menuText[i].setColor(sf::Color::Magenta);
    }
    selectedIndex = 0;

}

void Menu::draw(sf::RenderWindow &window)
{
    for(int i = 0; i < NUM_ITEMS; i++)
        window.draw(menuText[i]);
}

void Menu::MoveUp()
{
    if(selectedIndex > 0)
    {
        menuText[selectedIndex].setColor(sf::Color::Black);
        selectedIndex--;
        menuText[selectedIndex].setColor(sf::Color::Magenta);
    }
}

void Menu::MoveDown()
{
    if(selectedIndex < NUM_ITEMS - 1)
    {
        menuText[selectedIndex].setColor(sf::Color::Black);
        selectedIndex++;
        menuText[selectedIndex].setColor(sf::Color::Magenta);
    }
}

int Menu::GetSelected()
{
    return selectedIndex;
}

void Menu::SetMenuItems(int index, string text)
{
    menuText[index].setString(text);
}

void Menu::SetNumItems(int num)
{
    NUM_ITEMS = num;
}

int Menu::GetNumItems()
{
    return NUM_ITEMS;
}


//MAIN MENU
void menufunc(float width, float height)
{
    vector<string> menuitems;
    menuitems.push_back("Play");
    menuitems.push_back("Quit");
    // Create the main window
    sf::RenderWindow menuwindow(sf::VideoMode(width, height), "It's Menu Time!");
    menuwindow.setFramerateLimit(FRAME_RATE);
    menuwindow.setPosition(sf::Vector2i(0,0));

    Menu menu(width, height, menuitems.size());

    for(int i = 0; i<menuitems.size(); i++)
        {menu.SetMenuItems(i, menuitems[i]);};
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("Plinko4.png"))
        cout<<"Error loading Picture";
    sf::Sprite sprite(texture);
    sprite.setScale(.85, .98);

    //Load menu music
    sf::SoundBuffer menuBuffer, selectionBuffer;
//    if(!menuBuffer.loadFromFile("PlinkoMenu.ogg"))
    if(!menuBuffer.loadFromFile("plinkomenu.wav"))
        cout<<"Error loading sound";
    if(!selectionBuffer.loadFromFile("selection.wav"))
        cout<<"Error loading sound";
    sf::Sound menuMusic, selectionMusic;
    menuMusic.setBuffer(menuBuffer);
    menuMusic.play();
    menuMusic.setLoop(true);
    selectionMusic.setBuffer(selectionBuffer);


	// Start the game loop
    while (menuwindow.isOpen())
    {
        // Process events
        sf::Event event;
        while (menuwindow.pollEvent(event))
        {
            switch(event.type)
            {
            // Close window : exit
            case sf::Event::Closed:
                menuwindow.close();
                menuMusic.stop();
            case sf::Event::KeyReleased:
                switch(event.key.code)
                {
                    case sf::Keyboard::Up:
                        menu.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        menu.MoveDown();
                        break;

                    case sf::Keyboard::Return:
                        switch(menu.GetSelected())
                        {
                        case 0:
                            menuMusic.stop();
                            selectionMusic.play();
                            menuwindow.close();
                            gamefunc();
                        case 1:
                            menuMusic.stop();
                            selectionMusic.play();
                            menuwindow.close();
                        }
                }
            break;
            }
        }
        // Clear screen
        menuwindow.clear(sf::Color::White);

        //Draw menuwindow
        menuwindow.draw(sprite);
        menu.draw(menuwindow);
        // Update the window
        menuwindow.display();
    }

}

//PAUSE MENU
void pausefunc(float width, float height, sf::RenderWindow &pausewindow)
{
    vector<string> pauseitems;
    pauseitems.push_back("Resume");
    pauseitems.push_back("Main Menu");
    pauseitems.push_back("Quit");

    Menu pausemenu(width, height, pauseitems.size());

    for(int i = 0; i<pauseitems.size(); i++)
        {pausemenu.SetMenuItems(i, pauseitems[i]);};

    int mode = -1;
	// Start the game loop
    while (pausewindow.isOpen())
    {
        // Process events
        sf::Event event;
        while (pausewindow.pollEvent(event))
        {
            switch(event.type)
            {
            // Close window : exit
            case sf::Event::Closed:
                pausewindow.close();
            case sf::Event::KeyReleased:
                switch(event.key.code)
                {
                    case sf::Keyboard::Up:
                        pausemenu.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        pausemenu.MoveDown();
                        break;

                    case sf::Keyboard::Return:
                        switch(pausemenu.GetSelected())
                        {
                        case 0:
                            mode = 0;
                            break;
                        case 1:
                            pausewindow.close();
                            menufunc(width, height);
                        case 2:
                            pausewindow.close();
                        }
                }
            break;
            }
        }

        if(mode == 0)
            break;

        pausemenu.draw(pausewindow);
        pausewindow.display();
    }

}


