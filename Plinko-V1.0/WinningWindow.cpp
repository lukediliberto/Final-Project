#include "WinningWindow.h"
#include <SFML/Graphics.hpp>
#include "Menu.h"

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
//^^need to include this in every file that has audio!!!

using namespace std;

void winWindowFunc(float width, float height, int money)
{
    // Create the main window
    sf::RenderWindow winWindow(sf::VideoMode(width, height), "Winner!!!");
    winWindow.setFramerateLimit(60);

    // Load a sprite to display
    sf::Texture texture;
    texture.loadFromFile("winner.jpg"); //put possible failure statement here

    sf::Sprite sprite1(texture);
    sprite1.setPosition(0,0);

    sf::Text text1,text2,text3;
    // select the font
    sf::Font font;
    font.loadFromFile("PLINKO2K.TTF");

    text1.setFont(font);
    text1.setCharacterSize(24); // in pixels, not points!
    text1.setColor(sf::Color::Magenta);

    string tempMoney=to_string(money);
    string moneyString="$"+ tempMoney;

    //makes code less redundant
    text2=text1;
    text3=text1;

    text1.setString("Congratulations!!! You win");
    text1.setPosition(40,360);
    text2.setString(moneyString);
    text2.setPosition(220,384);
    text3.setString("Press Escape for Main Menu");
    text3.setPosition(20,460);
    text3.setColor(sf::Color::Black);

    //cheering sound
    sf::Music music1;
    music1.openFromFile("cheering.wav");
    music1.setVolume(100);
    //music1.setPitch(1);

    //"Shut up and take my money!" sound
    sf::Music music2;
    music2.openFromFile("fry.wav");
    music2.setVolume(80);
    //music2.setPitch(1);

    //used to determine timing of musics
    int counter=0;

////////////////////////////////////////////////////////////////////////////////////////

	// Start the game loop
    while (winWindow.isOpen())
    {
        // Process events
        sf::Event event;
        while (winWindow.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                winWindow.close();

            // Escape pressed: exit and return to main menu
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                music1.stop();
                music2.stop();
                winWindow.close();
                menufunc(500,500);
            }
        }

        //cheering sound first
        if (counter==0)
            music1.play();

        //"Shut up and take my money!"
        if (counter==275)
            music2.play();

        counter++;


        winWindow.clear(sf::Color::White);

        winWindow.draw(sprite1);
        winWindow.draw(text1);
        winWindow.draw(text2);
        winWindow.draw(text3);

        // Update the window
        winWindow.display();
    }
}

