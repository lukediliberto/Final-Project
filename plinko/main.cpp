#include <SFML/Graphics.hpp>
#include "PlinkoChip.h"
#include <iostream>
#include <vector>

using namespace std;


sf::Vector2f getFinalSpeed(sf::Vector2f v1, sf::Vector2f x1, sf::Vector2f x2)
{
    sf::Vector2f vdiff = v1;
    sf::Vector2f xdiff = x1-x2;

    float denom=xdiff.x*xdiff.x + xdiff.y*xdiff.y;
    float numerator=vdiff.x*xdiff.x+vdiff.y*xdiff.y;

    sf::Vector2f secondVector = xdiff*numerator/denom;

    sf::Vector2f result = v1-secondVector;
    return result;

}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(500, 500), "Plinko!!!");

    // Load a sprite to display
   /* sf::Texture texture;
    if (!texture.loadFromFile("cb.bmp"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture); */

    const int pegsPerLineOdd=9;
    const int pegsPerLineEven=10;

////////////////////////////////////////////////////////////////////////////////////////

    vector<sf::CircleShape> circleVecOdd;

    for (int i=0; i<36; i++)
    {
        circleVecOdd.push_back(sf::CircleShape());
        circleVecOdd[i].setRadius(5);
        circleVecOdd[i].setFillColor(sf::Color::Blue);
        circleVecOdd[i].setOrigin(sf::Vector2f(5,5));
    }

    int oddSIZE=circleVecOdd.size();

    int oddLine=0;

    for (int i=0; i<oddSIZE; i++)
    {
        int offset=50;

        if (i%pegsPerLineOdd==0 && i!=0)
        {
            oddLine++;
        }

        circleVecOdd[i].setPosition(offset+(i%pegsPerLineOdd)*50,50+100*oddLine);
    }
////////////////////////////////////////////////////////////////////////////////////////
    vector<sf::CircleShape> circleVecEven;

    for (int i=0; i<40; i++)
    {
        circleVecEven.push_back(sf::CircleShape());
        circleVecEven[i].setRadius(5);
        circleVecEven[i].setFillColor(sf::Color::Blue);
        circleVecEven[i].setOrigin(sf::Vector2f(5,5));
    }

    int evenSIZE=circleVecEven.size();

    int evenLine=0;

    for (int i=0; i<evenSIZE; i++)
    {
        int offset=25;

        if (i%pegsPerLineEven==0 && i!=0)
        {
            evenLine++;
        }

        circleVecEven[i].setPosition(offset+(i%pegsPerLineEven)*50,100+100*evenLine);
    }
////////////////////////////////////////////////////////////////////////////////////////

    //creating the main borders
    vector<sf::RectangleShape> borderRect;

    for (int i=0; i<3; i++)
    {
        borderRect.push_back(sf::RectangleShape());
        borderRect[i].setSize(sf::Vector2f(10,500));
        borderRect[i].setFillColor(sf::Color::Magenta);
    }

    int borderSIZE=borderRect.size();

    borderRect[1].setPosition(490,0);

    borderRect[2].setPosition(0,500);
    borderRect[2].setRotation(270);
////////////////////////////////////////////////////////////////////////////////////////

    //creating the bottom rectangles
    vector<sf::RectangleShape> bottomRect;

    for (int i=0; i<9; i++)
    {
        bottomRect.push_back(sf::RectangleShape());
        bottomRect[i].setSize(sf::Vector2f(10,50));
        bottomRect[i].setRotation(180);
        bottomRect[i].setFillColor(sf::Color::Cyan);
    }

    int bottomSIZE=bottomRect.size();

    for (int i=0; i<bottomSIZE; i++)
    {
        bottomRect[i].setPosition(55+50*i,500);
    }
////////////////////////////////////////////////////////////////////////////////////////

    //creating a PlinkChip
    PlinkoChip chip1=PlinkoChip();

    chip1.setPosition(220,25);
    chip1.setVelocity(0,4);

////////////////////////////////////////////////////////////////////////////////////////
	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Q pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
                window.close();
        }

        // Clear screen
        window.clear(sf::Color::White);

        // Draw the sprite
        for (int i=0; i<oddSIZE; i++)
            window.draw(circleVecOdd[i]);

        for (int i=0; i<evenSIZE; i++)
            window.draw(circleVecEven[i]);

        for (int i=0; i<borderSIZE; i++)
            window.draw(borderRect[i]);

        for (int i=0; i<bottomSIZE; i++)
            window.draw(bottomRect[i]);

        window.draw(chip1);

        // Update the window
        window.display();
        //set frame rate
        window.setFramerateLimit(60);
////////////////////////////////////////////////////////////////////////////////////////

        chip1.continueVelocity();



    //odd pegs
        for (int i=0; i<oddSIZE; i++)
        {
            sf::Rect<float> chipRect = chip1.getGlobalBounds();
            sf::Rect<float> RectOI = circleVecOdd[i].getGlobalBounds();

            if (chipRect.intersects(RectOI))
            {
                //change velocity accordingly
                sf::Vector2f position = chip1.getPosition();
                sf::Vector2f velocity = chip1.getVelocity();

                sf::Vector2f otherposition = circleVecOdd[i].getPosition();

                sf::Vector2f finalVelocity = getFinalSpeed(velocity,position,otherposition);

                chip1.setVelocity(finalVelocity.x,finalVelocity.x);

            }
        }

    //even pegs
        for (int i=0; i<evenSIZE; i++)
        {
            sf::Rect<float> chipRect = chip1.getGlobalBounds();
            sf::Rect<float> RectOI = circleVecEven[i].getGlobalBounds();

            if (chipRect.intersects(RectOI))
            {
                //change velocity accordingly
                sf::Vector2f position = chip1.getPosition();
                sf::Vector2f velocity = chip1.getVelocity();

                sf::Vector2f otherposition = circleVecEven[i].getPosition();

                sf::Vector2f finalVelocity = getFinalSpeed(velocity,position,otherposition);

                chip1.setVelocity(finalVelocity.x,finalVelocity.x);
            }
        }

    //bottom rectangles
        for (int i=0; i<bottomSIZE; i++)
        {
            sf::Rect<float> chipRect = chip1.getGlobalBounds();
            sf::Rect<float> RectOI = bottomRect[i].getGlobalBounds();

            if (chipRect.intersects(RectOI))
            {
                //change velocity accordingly
                sf::Vector2f position = chip1.getPosition();
                sf::Vector2f velocity = chip1.getVelocity();

                sf::Vector2f otherposition = bottomRect[i].getPosition();

                sf::Vector2f finalVelocity = getFinalSpeed(velocity,position,otherposition);

                chip1.setVelocity(finalVelocity.x,finalVelocity.x);
            }
        }

    //border rectangles
        for (int i=0; i<borderSIZE; i++)
        {
            sf::Rect<float> chipRect = chip1.getGlobalBounds();
            sf::Rect<float> RectOI = borderRect[i].getGlobalBounds();

            if (chipRect.intersects(RectOI))
            {
                //change velocity accordingly
                sf::Vector2f position = chip1.getPosition();
                sf::Vector2f velocity = chip1.getVelocity();

                sf::Vector2f otherposition = borderRect[i].getPosition();

                sf::Vector2f finalVelocity = getFinalSpeed(velocity,position,otherposition);

                chip1.setVelocity(finalVelocity.x,finalVelocity.x);
            }
        }

        //show velocities
        sf::Vector2f velocity=chip1.getVelocity();
        cout << velocity.x << " " << velocity.y << endl;

        //how to implement gravity?




////////////////////////////////////////////////////////////////////////////////////////
    }

    return EXIT_SUCCESS;
}
