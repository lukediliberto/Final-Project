#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "PlinkoChip.h"
#include "Peg.h"
#include "MomentumTransfer.h"
#include "Menu.h"


#include <SFML/Audio.hpp>
#include <string>

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

void gamefunc(char c)
{
    int maxNumberOfChips;
    int totalMoney=0;
    int chipCount=0;

    int FRAME_RATE = 60;
    int window_width = 500;
    int window_height = 700;
    bool windowbound = true;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Plinko!!!");
    window.setPosition(sf::Vector2i(0,0));

    //Background for gameboard. Still messing around with this...[Luke]
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("Background.png");
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.7,1.7);
    backgroundSprite.setColor(sf::Color(0, 0, 0, 0));

    // select the font
    sf::Font font;
    if(!font.loadFromFile("PLINKO2K.TTF"))
        cout<<"Error loading font (Bin Strings)"<<endl;

////////////////////////////////////////////////////////////////////////////////////////
    sf::Text chipText, moneyText;

    chipText.setFont(font);
    chipText.setCharacterSize(24);
    chipText.setColor(sf::Color::Magenta);

    moneyText.setFont(font);
    moneyText.setCharacterSize(24);
    moneyText.setColor(sf::Color::Magenta);

////////////////////////////////////////////////////////////////////////////////////////

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

    //creating a PlinkoChip
    PlinkoChip testChip(0,0,260,0,windowbound,window_width,window_height);
    //create texture for plinkochip
    //texture stuff:
    sf::Texture texture;
    texture.loadFromFile("plinkoimage.png");
    testChip.setTexture(&texture);
    //sounds
    sf::Sound sound, selectionMusic, gamebeep, gamemusic;
    sf::SoundBuffer buffer, gameplayBuffer, beepBuffer, selectionBuffer;
    buffer.loadFromFile("boing.wav");
    sound.setBuffer(buffer);
    //sf::SoundBuffer gameplayBuffer, beepBuffer, selectionBuffer;
    if(!gameplayBuffer.loadFromFile("gamemelody.wav"))
        cout<<"Error loading sound";
    if(!beepBuffer.loadFromFile("beep.ogg"))
        cout<<"Error loading sound";
    if(!selectionBuffer.loadFromFile("selection2.wav"))
        cout<<"Error loading sound";
    selectionMusic.setBuffer(selectionBuffer);
    gamebeep.setBuffer(beepBuffer);
    gamemusic.setBuffer(gameplayBuffer);
    gamemusic.play();
    gamemusic.setLoop(true);
    selectionMusic.play();

    // Load a sprite to display
    sf::Texture texture2;
    texture2.loadFromFile("scoreboard.png"); //put possible failure statement here

    sf::Sprite scoreboard(texture2);
    scoreboard.setPosition(175,525);

    string initialMoney=to_string(totalMoney);

    moneyText.setString("$"+initialMoney);
    moneyText.setColor(sf::Color::Black);
    moneyText.setPosition(195,550);

    string initalChips=to_string(maxNumberOfChips);

    chipText.setString(initalChips);
    chipText.setPosition(283,638);
    chipText.setColor(sf::Color::Black);


////////////////////////////////////////////////////////////////////////////////////////
    vector<Peg> allPegs;
    int pegNumber=0;

    int colorCounter=0; //for intializing the vector for colors
    vector<int> bounceCount;

    bool isInBin=0;
    bool mouseWasPressed=0;
    bool chipCanDrop=1;

    //count if chip set
    int counter = 0;
    //Error message for low chip placement
    sf::Font errorFont;
    if(!errorFont.loadFromFile("PLINKO2K.TTF"))
        cout<<"Error loading Font (game Funciton)"<<endl;
    sf::Text chipError("", errorFont, 20);
    chipError.setPosition(window_width/4.5, window_height/4.2);
    chipError.setColor(sf::Color::Magenta);

////////////////////////////////////////////////////////////////////////////////////////
    //if O was pressed
    if(c == 'O')
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

            //Creating bottom pegs for prize bins
            vector<Peg> bottomPegs;
            for (int i=0; i<250; i++)
            {
                bottomPegs.push_back(Peg());
                bottomPegs[i].setPosition(i*2,505);
            }


            //Putting all pegs into allPegs
            allPegs.insert(allPegs.end(),bottomPegs.begin(),bottomPegs.end());
            allPegs.insert(allPegs.end(),binPegs.begin(),binPegs.end());
            allPegs.insert(allPegs.end(),evenPegs.begin(),evenPegs.end());
            allPegs.insert(allPegs.end(),oddPegs.begin(),oddPegs.end());

            pegNumber=allPegs.size();
    }

        //if R was pressed
        if (c == 'R')
        {
            allPegs.clear();

            vector<Peg> pegVec;
            for (int i=0; i<65; i++)
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

            //Creating bottom pegs for prize bins
            vector<Peg> bottomPegs;
            for (int i=0; i<250; i++)
            {
                bottomPegs.push_back(Peg());
                bottomPegs[i].setPosition(i*2,505);
            }

            //Putting all pegs into allPegs
            allPegs.insert(allPegs.end(),bottomPegs.begin(),bottomPegs.end());
            allPegs.insert(allPegs.end(),binPegs.begin(),binPegs.end());
            allPegs.insert(allPegs.end(),pegVec.begin(),pegVec.end());

            pegNumber=allPegs.size();
        }
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

            //P Pressed: Pause
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                pausefunc(window_width, window_height, window);

            //Mouse moved
            if(event.type == sf::Event::MouseMoved && counter == 0)
                testChip.setPosition(event.mouseMove.x, event.mouseMove.y);

            //Mouse pressed
            if(event.type == sf::Event::MouseButtonPressed && testChip.getPosition().y >= 20 && counter == 0)
                {chipError.setString("Chip Must Be Dropped\n \n    at top of Board");
				gamebeep.play();};
            if(event.type == sf::Event::MouseButtonPressed && testChip.getPosition().y < 20 && chipCanDrop)
            {
                mouseWasPressed=1;
                chipCount++;
                chipText.setString(to_string(maxNumberOfChips-chipCount));
                chipError.setString("");
            }
        }

////////////////////////////////////////////////////////////////////////////////////////
        // Clear screen
        window.clear(sf::Color::White);

        window.draw(backgroundSprite);

        // Draw the sprite
        for (int i=0; i<allPegs.size(); i++)
            {window.draw(allPegs[i]);}
        window.draw(testChip);
        window.draw(chipError);

        for(int i = 0; i<9; i++)
            window.draw(binMoney[i]);

        window.draw(scoreboard);
        window.draw(chipText);
        window.draw(moneyText);

        // Update the window
        window.display();
        //set frame rate
        window.setFramerateLimit(FRAME_RATE);
////////////////////////////////////////////////////////////////////////////////////////

        if(mouseWasPressed)
        {
            counter=1;
            isInBin=0;
        }

////////////////////////////////////////////////////////////////////////////////////////
    //main function
        if(counter != 0)
        {

            //check for collisions with all pegs except bottom pegs
            for(int i = 250; i<allPegs.size();i++)
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

            //if the chip lands on a bottom peg, stop it and increment delay counter
            for(int i=0; i<250; i++)
            {
                if(getDistance(testChip.getNextPosition(),allPegs[i].getPosition())<=
                                (testChip.getRadius()+allPegs[i].getRadius()))
                {
                    testChip.setVelocity(0,0);
                    testChip.setPosition(position.x,505+0.5-allPegs[i].getRadius()-testChip.getRadius());
                }
            }

            double binwidth=55.5;
            int money=0;

        //don't know why it should be this #, but it works
            if(position.y>=485)
            {
                gamemusic.stop();

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
                else if (position.x>=7*binwidth && position.x<=8*binwidth)
                    money=500;
                else
                    money=100;

                totalMoney=totalMoney+money;
                moneyText.setString("$"+to_string(totalMoney));
            }

            //initializing bounceCount vector
            if (colorCounter==0)
                bounceCount.resize(pegNumber,0);
                //kept the index of i the same by making the vector to include all pegs

            //changing pegs that have a collision with the chip
            for (int i=350; i<pegNumber; i++)
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

                    bounceCount[i]++;
                    colorCounter++;
                }
            }

            if(isInBin)
            {
                if (chipCount==maxNumberOfChips) //this occurs when you run out of chips
                {
                    chipCanDrop=0;
                    winfunc(window_width, window_height, window, c, totalMoney, allPegs);
                }
                chipError.setString("Move Mouse to Top of Board\n \n    to Drop Next Chip");
                counter=0;
                mouseWasPressed=0;
            }
        }
    }
//    return EXIT_SUCCESS;
}
