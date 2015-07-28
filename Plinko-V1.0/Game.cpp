#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "PlinkoChip.h"
#include "Peg.h"
#include "MomentumTransfer.h"
#include "Menu.h"
#include "WinningWindow.h"

#include <SFML/Audio.hpp>

using namespace std;

//this function is used to ensure that each stone has a unique position
void checkForUniquePosition (vector<Peg> &pegVec) //need to pass vector by reference
{
    int pegSize = pegVec.size();
    //int chipDiameter=30; //need to make this PlinkoChip().getRadius()*2


    for (int i=0; i<pegSize; i++)
    {
        for (int j=0; j<pegSize; j++)
        {
            sf::Vector2f position = pegVec[i].getPosition();
            sf::Vector2f otherPosition = pegVec[j].getPosition();

            float xdistanceSquared=(position.x-otherPosition.x)*(position.x-otherPosition.x);
            float ydistanceSquared=(position.y-otherPosition.y)*(position.y-otherPosition.y);

            float distance=sqrt(xdistanceSquared+ydistanceSquared);

            double collisionDistance=45;
            //2*pegVec[i].getRadius()+chipDiameter;

            //if a stone is to be created too close to a different stone, then reset this process
            if(distance<=collisionDistance && i!=j)
            {
                pegVec[i].setRandomPosition();
                j=-1; // because j++ before it repeats through the loop
            }
        }
    }
}

void gamefunc()
{
    int FRAME_RATE = 60;
    int window_width = 500;
    int window_height = 500;
    bool windowbound = true;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Plinko!!!");
    window.setPosition(sf::Vector2i(0,0));

    //initializing
    bool Opressed=0;
    bool Rpressed=0;

    //to make sure that button can only pressed once
    int Ocount=0;
    int Rcount=0;

    bool isInBin=0;

    bool noKeysPressed=1;

    sf::Text text1,text2,text3, errorText;
    // select the font
    sf::Font font;
    font.loadFromFile("PLINKO2K.TTF");

    text1.setFont(font);
    text1.setCharacterSize(24); // in pixels, not points!
    text1.setColor(sf::Color::Magenta);

    text2=text1;
    text3=text1;
    errorText=text1;

    sf::Text binMoney[9];
    binMoney[0].setString("$\n1\n0\n0");
    binMoney[1].setString("$\n5\n0\n0");
    binMoney[2].setString("$\n1\n0\n0\n0");
    binMoney[3].setString("$\n0");
    binMoney[4].setString("$\n1\n0\n0\n0\n0");
    binMoney[4].setPosition(30+(45*4)+(12*(4-1)),425);
    binMoney[5].setString("$\n0");
    binMoney[6].setString("$\n1\n0\n0\n0");
    binMoney[7].setString("$\n5\n0\n0");
    binMoney[8].setString("$\n1\n0\n0");
    for(int i = 0; i<9; i++)
    {
        binMoney[i].setFont(font);
        binMoney[i].setColor(sf::Color::Green);
        binMoney[i].setCharacterSize(12);
        if(i != 4)
            binMoney[i].setPosition(30+(45*i)+(12*(i-1)),435);
    }


    text1.setString("To select Game mode, press:");
    text1.setPosition(20,100);
    text2.setString("O for Original");
    text2.setPosition(125,150);
    text3.setString("R for Random");
    text3.setPosition(125,180);


    //Combine all pegs into single vector for easy processing
    // -- TBD Possible to add into allPegs as created, make easier or harder to follow???
    vector<Peg> allPegs;

    int pegNumber=0;


    //creating a PlinkoChip
    PlinkoChip testChip(0,0,260,0,windowbound,window_width,window_height);
    //create texture for plinkochip
    //texture stuff:
    sf::Texture texture;
    texture.loadFromFile("plinkoimage.png");
    testChip.setTexture(&texture);

    //sounds
    sf::SoundBuffer buffer;
    buffer.loadFromFile("boing.wav");
    sf::Sound sound;
    sound.setBuffer(buffer);
    sf::SoundBuffer gameplayBuffer, beepBuffer, selectionBuffer;
    sf::Sound selectionMusic;
    selectionMusic.setBuffer(selectionBuffer);
    sf::Sound gamebeep;
    gamebeep.setBuffer(beepBuffer);
    sf::Sound gamemusic;
    gamemusic.setBuffer(gameplayBuffer);
    gamemusic.play();
    gamemusic.setLoop(true);


    int colorCounter=0; //for intializing the vector for colors
    vector<int> bounceCount;

    bool bounceInPlaceError=0;

    errorText.setString("Error! Quitting to Main Menu...");
    errorText.setPosition(20,100);


////////////////////////////////////////////////////////////////////////////////////////


    //count if chip set
    int counter = 0;
    //Error message for low chip placement
    sf::Font errorFont;
    if(!errorFont.loadFromFile("PLINKO2K.TTF"))
        cout<<"Error loading Font (game Funciton)"<<endl;
    sf::Text chipError("", errorFont, 20);
    chipError.setPosition(window_width/4.5, window_height/4.2);
    chipError.setColor(sf::Color::Magenta);
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

            //P Pressed: Pause
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                pausefunc(window_width, window_height, window);
            //comment this line out to test mouse
            //counter = 1;
            //uncomment to test mouse
            if(event.type == sf::Event::MouseMoved && counter == 0)
                testChip.setPosition(event.mouseMove.x, event.mouseMove.y);
            if(event.type == sf::Event::MouseButtonPressed && testChip.getPosition().y >= 20 && counter == 0)
                {chipError.setString("Chip Must Be Dropped\n \n    at top of Board");
				gamebeep.play();};
            if(event.type == sf::Event::MouseButtonPressed && testChip.getPosition().y < 20)
            {
                //NOTE its possible to drop the chip straight down on a peg and only bounce
                //up and down
                counter = 1;
                chipError.setString("");
            }
////////////////////////////////////////////////////////////////////////////////////////

            //if O is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {
				selectionMusic.play();
                Opressed=1;
                Rpressed=0;
                noKeysPressed=0;
            }

            //if R is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
				selectionMusic.play();
                Opressed=0;
                Rpressed=1;
                Rcount=0;
                noKeysPressed=0;
            }
////////////////////////////////////////////////////////////////////////////////////////

            //if O was pressed
            if (Opressed && Ocount==0) //so it only occurs once
            {
                allPegs.clear();

                const int pegsPerLineOdd=9;
                const int pegsPerLineEven=10;

                //creating oddPegs vector
                vector<Peg> oddPegs;
                for (int i=0; i<36; i++)
                    {oddPegs.push_back(Peg());}
                int oddSIZE=oddPegs.size();
                int oddLine=0;
                for (int i=0; i<oddSIZE; i++)
                {
                    int offset=50;
                    if (i%pegsPerLineOdd==0 && i!=0)
                        {oddLine++;}
                    oddPegs[i].setPosition(offset+(i%pegsPerLineOdd)*50,50+100*oddLine);
                }

                //creating evenPegs vector
                vector<Peg> evenPegs;
                for (int i=0; i<40; i++)
                    {evenPegs.push_back(Peg());}
                int evenSIZE=evenPegs.size();
                int evenLine=0;
                for (int i=0; i<evenSIZE; i++)
                {
                    int offset=25;
                    if (i%pegsPerLineEven==0 && i!=0)
                        {evenLine++;}
                    evenPegs[i].setPosition(offset+(i%pegsPerLineEven)*50,100+100*evenLine);
                }

                //Create Prize Bins
                vector<Peg> binPegs;
                for (int i=0; i<100; i++)
                    {binPegs.push_back(Peg());}
                int binWalls = 10;
                for (int i=0; i<10; i++)
                {
                    for(int j = 0; j<binWalls ; j++)
                    {
                        binPegs[i*binWalls+j].setPosition(j*55.5,500-i*4);
                    }
                }

                //putting all pegs into allPegs
                allPegs.insert(allPegs.end(),binPegs.begin(),binPegs.end());
                allPegs.insert(allPegs.end(),evenPegs.begin(),evenPegs.end());
                allPegs.insert(allPegs.end(),oddPegs.begin(),oddPegs.end());

                Ocount++;
                Rcount=0;
                pegNumber=176;
            }

            //if R was pressed
            if (Rpressed && Rcount==0)
            {
                allPegs.clear();

                vector<Peg> pegVec;
                for (int i=0; i<60; i++)
                {
                    pegVec.push_back(Peg());
                    pegVec[i].setRandomPosition();
                }
                checkForUniquePosition(pegVec);

                //Create Prize Bins
                vector<Peg> binPegs;
                for (int i=0; i<100; i++)
                    {binPegs.push_back(Peg());}
                int binWalls = 10;
                for (int i=0; i<10; i++)
                {
                    for(int j = 0; j<binWalls ; j++)
                    {
                        binPegs[i*binWalls+j].setPosition(j*55.5,500-i*4);
                    }
                }

                //putting all pegs into allPegs
                allPegs.insert(allPegs.end(),binPegs.begin(),binPegs.end());
                allPegs.insert(allPegs.end(),pegVec.begin(),pegVec.end());

                Ocount=0;
                Rcount++;
                pegNumber=160;
            }
        }

////////////////////////////////////////////////////////////////////////////////////////
    //main function
        if(counter != 0)
        {
            //check for collisions.
            for(int i = 0; i<allPegs.size();i++)
            {
                if(getDistance(testChip.getNextPosition(),allPegs[i].getPosition())<=
                                (testChip.getRadius()+allPegs[i].getRadius()))
                        {MomentumTransfer(testChip,allPegs[i]);}
            }
            //update position of Chip
            testChip.setNextPosition();
            testChip.setPosition(testChip.getNextPosition().x,testChip.getNextPosition().y);
            testChip.applyGravity();

            //if the chip reaches a bin:
            sf::Vector2f position = testChip.getPosition();
            sf::Vector2f velocity = testChip.getVelocity();

            double binwidth=55.5;
            int money=0;


        //this occurs when PlinkoChip lands in a bin
        //trying to make the transition a little slower
        //I want to see the chip really land in the bin
            if(position.y>=475.000001 && velocity.y<=0.000000000000001)
            {
                gamemusic.stop();
                testChip.setVelocity(0,0);
                isInBin=1;

                //determining the amount of money awarded
                if (position.x>=0 && position.x<=binwidth)
                    money=100;
                else if (position.x>=binwidth && position.x<=2*binwidth)
                    money=500;
                else if (position.x>=2*binwidth && position.x<=3*binwidth)
                    money=1000;
                else if (position.x>=3*binwidth && position.x<=4*binwidth)
                    money=0;
                else if (position.x>=4*binwidth && position.x<=5*binwidth)
                    money=10000;
                else if (position.x>=5*binwidth && position.x<=6*binwidth)
                    money=0;
                else if (position.x>=6*binwidth && position.x<=7*binwidth)
                    money=1000;
                else if (position.x>=7*binwidth && position.x<=7*binwidth)
                    money=500;
                else
                    money=100;
            }

            if(isInBin)
            {
                window.close();
                winWindowFunc(window_width, window_height, money);
            }

            //initializing bounceCount vector
            if (colorCounter==0)
                bounceCount.resize(pegNumber,0);
                //kept the index of i the same by making the vector to include all pegs


            //changing pegs that have a collision with the chip
            for (int i=100; i<pegNumber; i++)
            {
                if(getDistance(testChip.getNextPosition(),allPegs[i].getPosition())<=
                        (testChip.getRadius()+allPegs[i].getRadius()))
                {
                    //make boing sound
                    sound.play();

                    //change color of peg depending on number of bounces on same peg
                    //have it rotate through same few colors (total: 5)
                    if (bounceCount[i]%5==0)
                        allPegs[i].setFillColor(sf::Color::Magenta);
                    else if (bounceCount[i]%5==1)
                        allPegs[i].setFillColor(sf::Color::Red);
                    else if (bounceCount[i]%5==2)
                        allPegs[i].setFillColor(sf::Color::Green);
                    else if (bounceCount[i]%5==3)
                        allPegs[i].setFillColor(sf::Color::Blue);
                    else
                        allPegs[i].setFillColor(sf::Color::Cyan);


                //case where the chip gets "stuck" bouncing in the same spot
                    if (bounceCount[i]>=15)
                        bounceInPlaceError=1;

                    if (bounceCount[i]>=40)
                    {
                        window.close();
                        bounceInPlaceError=0;
                        menufunc(500,500);
                    }

                    bounceCount[i]++;
                    colorCounter++;
                }
            }
        }

        // Clear screen
        window.clear(sf::Color::White);
        // Draw the sprite
        for (int i=0; i<allPegs.size(); i++)
            {window.draw(allPegs[i]);}
        window.draw(testChip);
        window.draw(chipError);

        if (noKeysPressed)
        {
            window.draw(text1);
            window.draw(text2);
            window.draw(text3);
        }
	for(int i = 0; i<9; i++)
            window.draw(binMoney[i]);

        if (bounceInPlaceError)
            window.draw(errorText);

        // Update the window
        window.display();
        //set frame rate
        window.setFramerateLimit(FRAME_RATE);
    }

//    return EXIT_SUCCESS;
}
