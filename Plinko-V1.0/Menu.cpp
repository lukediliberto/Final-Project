#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

//#include <thread>         // std::this_thread::sleep_for
//#include <chrono>         // std::chrono::seconds
#include <unistd.h>

using namespace std;

int FRAME_RATE = 10;

Menu::Menu(float width, float height, int num, sf::Color color = sf::Color::Magenta, unsigned int textsize = 30)
{
    NUM_ITEMS = num;
    textcolor = color;

    if(!font.loadFromFile("PLINKO2K.TTF"))
        cout<<"Error loading Font (Menu Constructor)"<<endl;

    for(int i = 0; i<NUM_ITEMS; i++)
    {
        menuText[i].setFont(font);
        menuText[i].setColor(sf::Color::Black);
        menuText[i].setPosition(sf::Vector2f(width/30, height-150 + 30*i));
        menuText[i].setCharacterSize(textsize);
        if(i == 0)
            menuText[i].setColor(color);
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
        menuText[selectedIndex].setColor(textcolor);
    }
}

void Menu::MoveDown()
{
    if(selectedIndex < NUM_ITEMS - 1)
    {
        menuText[selectedIndex].setColor(sf::Color::Black);
        selectedIndex++;
        menuText[selectedIndex].setColor(textcolor);
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

void Menu::SetTextPosition(int index, float x, float y)
{
    menuText[index].setPosition(sf::Vector2f(x,y));
}

void Menu::SetTextColor(sf::Color color)
{
    textcolor = color;
}


//MAIN MENU
void menufunc(float width, float height)
{
    vector<string> menuitems;
    menuitems.push_back("Play");
    //menuitems.push_back("Play Random");
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
                            //menuwindow.close();
                            subfunc(width, height, menuwindow, menu);
                            break;
                        /*case 1:
                            menuMusic.stop();
                            selectionMusic.play();
                            menuwindow.close();
                            gamefunc('r');
                            break;*/
                        case 1:
                            menuMusic.stop();
                            menuwindow.close();
                            break;
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

void subfunc(float width, float height, sf::RenderWindow &submenuwindow, Menu menu)
{
    vector<string> subitems;
    subitems.push_back("Play Original");
    subitems.push_back("Play Random");
    subitems.push_back("Back");

    submenuwindow.setPosition(sf::Vector2i(0,0));

    Menu submenu(width, height, subitems.size(), sf::Color::Blue, 20);
    for(int i = 0; i<3; i++)
    {   submenu.SetTextPosition(i, width/2.5, height*3.2/5 + 30*i);}

    for(int i = 0; i<subitems.size(); i++)
        {submenu.SetMenuItems(i, subitems[i]);};

    // Load a sprite to display
    /*sf::Texture texture;
    if (!texture.loadFromFile("SubMenu.png"))
        cout<<"Error loading Picture (SubMenu)";
    sf::Sprite sprite(texture);
    sprite.setScale(10, .082);
    sprite.setPosition(width/3.6, height*2.8/5 + 35);*/

    //sprite.setScale(.30, .1);
    //sprite.setPosition(width/4.8, height*2.8/5 + 35);

    int mode = -1;
	// Start the game loop
    while (submenuwindow.isOpen())
    {
        // Process events
        sf::Event event;
        while (submenuwindow.pollEvent(event))
        {
            switch(event.type)
            {
            // Close window : exit
            case sf::Event::Closed:
                submenuwindow.close();
            case sf::Event::KeyReleased:
                switch(event.key.code)
                {
                    case sf::Keyboard::Up:
                        submenu.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        submenu.MoveDown();
                        break;

                    case sf::Keyboard::Return:
                        switch(submenu.GetSelected())
                        {
                        case 0:
                            submenuwindow.close();
                            gamefunc('O');
                            break;
                        case 1:
                            submenuwindow.close();
                            gamefunc('R');
                            break;
                        case 2:
                            mode = 0;
                            break;
                        }
                }
            break;
            }
        }

        if(mode == 0)
            break;
        // Clear screen
        //Draw menuwindow
        //submenuwindow.draw(sprite);
        submenu.draw(submenuwindow);
        // Update the window
        submenuwindow.display();
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
                            break;
                        case 2:
                            pausewindow.close();
                            break;
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

void winfunc(float width, float height, sf::RenderWindow &winwindow, char c, int cash, vector<Peg> allPegs)
{
    vector<string> winitems;
    winitems.push_back("Play Again");
    winitems.push_back("Main Menu");
    winitems.push_back("Quit");

    Menu winmenu(width, height, winitems.size(), sf::Color::Green);

    for(int i = 0; i<winitems.size(); i++)
        {winmenu.SetMenuItems(i, winitems[i]);};

    sf::Text text1;
    // select the font
    sf::Font font;
    if(!font.loadFromFile("PLINKO2K.TTF"));
        cout<<"Error loading font (WinFunc)"<<endl;

    string money=to_string(cash);
    if(cash != 0)
        money="Congratulations!!!\n\n   You win: $"+ money;
    else
        money="You win $0\nBetter Luck Next Time";//POSTION THIS TEXT IN CENTER
    text1.setFont(font);
    text1.setCharacterSize(24);
    text1.setColor(sf::Color::Green);
    text1.setString(money);
    text1.setPosition(width/4, height/8);

    ifstream infile;
    infile.open("highscoresOrig.txt");
    string line;
    sf::Text high("HIGHSCORES", font, 30);
    high.setPosition(width/3.15, height/3);
    high.setColor(sf::Color::Black);

    //add check to see if score should be adde to high score list.
    //if so edit file (allow user to enter text in window)
    //add highscores for random mode.
    sf::Text scores[10];
    for(int i = 0; i<10; i++)
    {
        getline(infile,line);
        line = to_string(i+1) + ". "+ line;
        scores[i].setString(line);
        scores[i].setFont(font);
        scores[i].setCharacterSize(20);
        scores[i].setColor(sf::Color::Black);
        if(i<5)
            scores[i].setPosition(width/10, height/4 +100 +i*20);
        else
            scores[i].setPosition(width*3/5, height/4 + i*20);
    }

    int counter = 5;
	// Start the game loop
    while (winwindow.isOpen())
    {
        // Process events
        sf::Event event;
        while (winwindow.pollEvent(event))
        {
            switch(event.type)
            {
            // Close window : exit
            case sf::Event::Closed:
                winwindow.close();
            case sf::Event::KeyReleased:
                switch(event.key.code)
                {
                    case sf::Keyboard::Up:
                        winmenu.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        winmenu.MoveDown();
                        break;

                    case sf::Keyboard::Return:
                        switch(winmenu.GetSelected())
                        {
                        case 0:
                            winwindow.close();
                            gamefunc(c);
                            break;
                        case 1:
                            winwindow.close();
                            menufunc(width, height);
                            break;
                        case 2:
                            winwindow.close();
                            break;
                        }
                }
            break;
            }
        }
        //Change peg colors, slows down menu functionality
        for(int i = 0; i<allPegs.size(); i++)
        {
            //sleep and change pegs, un-comment usleep below to test
            if(counter % 2 == 0 && i % 2 == 0)
            {
                allPegs[i].setFillColor(sf::Color::Cyan);
                text1.setColor(sf::Color::Green);
            }
            if(counter % 2 == 0 && i % 2 == 1)
            {
                allPegs[i].setFillColor(sf::Color::Blue);
                text1.setColor(sf::Color::Green);
            }
            if(counter % 2 == 1 && i % 2 == 0)
            {
                 allPegs[i].setFillColor(sf::Color::Blue);
                 text1.setColor(sf::Color::Yellow);
            }
            if(counter % 2 == 1 && i % 2 == 1)
            {
                allPegs[i].setFillColor(sf::Color::Cyan);
                text1.setColor(sf::Color::Yellow);
            }
            winwindow.draw(allPegs[i]);
       }
        usleep(200000);
        counter++;

         winwindow.draw(text1);
        winwindow.draw(high);
        for(int i = 0; i<10; i++)
        {winwindow.draw(scores[i]);}

        //for (int i=0; i<allPegs.size(); i++)
           // {winwindow.draw(allPegs[i]);}
        winmenu.draw(winwindow);
        winwindow.display();
    }

}



