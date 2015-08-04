#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "PlinkoChip.h"
#include "Peg.h"
#include "MomentumTransfer.h"
#include "Menu.h"
#include "Mute.h"
#include "ripple.h"
#include <SFML/Audio.hpp>
#include <string>

using namespace std;

//this function is used to ensure that each peg has a unique position
void checkForUniquePosition (vector<Peg> &pegVec);
bool checkForValidBinPosition(PlinkoChip testChip);
vector<Peg> setupOriginalBoard();
vector<Peg> setupRandomBoard();
vector<Peg> setupBinPegs();
vector<Peg> setupBottomPegs();

void gamefunc(char c, Mute &mute)
{
    int maxNumberOfChips=3;
    int totalMoney=0;
    int chipCount=0;
    int round = 0;

    int FRAME_RATE = 30;
    int window_width = 500;
    int window_height = 685;
    bool windowbound = true;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Plinko!!!");
    window.setPosition(sf::Vector2i(0,0));

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
    //set strings for each bin
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
    sf::Texture chipTexture;
    chipTexture.loadFromFile("plinkoimage.png");
    testChip.setTexture(&chipTexture);

    //sounds
    sf::Sound collisionSound, selectionMusic, gamebeep, gamemusic;
    mute.muteSet(gamemusic);
    sf::SoundBuffer collisionBuffer, gameplayBuffer, beepBuffer, selectionBuffer;
    collisionBuffer.loadFromFile("boing.wav");
    collisionSound.setBuffer(collisionBuffer);
    gameplayBuffer.loadFromFile("gamemelody.wav");
    beepBuffer.loadFromFile("beep.ogg");
    selectionBuffer.loadFromFile("selection2.wav");
    selectionMusic.setBuffer(selectionBuffer);
    gamebeep.setBuffer(beepBuffer);
    gamemusic.setBuffer(gameplayBuffer);
    gamemusic.play();
    gamemusic.setLoop(true);
    selectionMusic.play();

    // Load a sprite to display
    sf::Texture scoreboardTexture;
    scoreboardTexture.loadFromFile("scoreboard.png"); //put possible failure statement here
    sf::Texture britaTexture;
    britaTexture.loadFromFile("brita.png");
    sf::Texture mopTexture;
    mopTexture.loadFromFile("LibmanMop.png");
    sf::Texture leftPlinkoDoorTexture;
    leftPlinkoDoorTexture.loadFromFile("leftplinkodoor.png");
    sf::Texture rightPlinkoDoorTexture;
    rightPlinkoDoorTexture.loadFromFile("rightplinkodoor.png");

    sf::Sprite scoreboard(scoreboardTexture);
    scoreboard.setPosition(175,510);

    sf::Sprite brita(britaTexture);
    brita.setPosition(335,513);
    brita.setScale(.5,.5);

    sf::Sprite leftPlinkoDoor(leftPlinkoDoorTexture);
    leftPlinkoDoor.setPosition(5,513);
    leftPlinkoDoor.setScale(.185,.17);

    sf::Sprite mop(mopTexture);
    mop.setPosition(5,525);
    mop.setScale(.3,.4);

    sf::Sprite rightPlinkoDoor(rightPlinkoDoorTexture);
    rightPlinkoDoor.setScale(.185,.17);
    rightPlinkoDoor.setOrigin(415,599);
    rightPlinkoDoor.setPosition(408,614);

    //set money won and chips remaining
    string initialMoney=to_string(totalMoney);

    moneyText.setString("$"+initialMoney);
    moneyText.setColor(sf::Color::Black);
    moneyText.setPosition(195,535);

    string initalChips=to_string(maxNumberOfChips);

    chipText.setString(initalChips);
    chipText.setPosition(283,623);
    chipText.setColor(sf::Color::Black);
////////////////////////////////////////////////////////////////////////////////////////
    vector<Peg> boardPegs;
    vector<Peg> binPegs;
    vector<Peg> bottomPegs;
    vector<Peg> allPegs;
    vector<Ripple> ripples;

    int colorCounter=0; //for intializing the vector for colors
    vector<int> bounceCount;

    //initializing
    bool finishedDropping=0;
    bool isInBin=0;
    bool mouseWasPressed=0;
    bool chipCanDrop=1;

    //count if chip set
    int counter = 0;
    //Error message for low chip placement
    sf::Text chipError("", font, 20);
    chipError.setPosition(window_width/4.5, window_height/4.2);
    chipError.setColor(sf::Color::Blue);

////////////////////////////////////////////////////////////////////////////////////////
    //Set up the Board and Bins

    //original game
    if(c == 'O')
        {boardPegs = setupOriginalBoard();}
    //random game
    if (c == 'R')
        {boardPegs = setupRandomBoard();}

    binPegs = setupBinPegs();
    bottomPegs = setupBottomPegs();
    allPegs.insert(allPegs.end(),bottomPegs.begin(),bottomPegs.end());
    allPegs.insert(allPegs.end(),binPegs.begin(),binPegs.end());
    allPegs.insert(allPegs.end(),boardPegs.begin(),boardPegs.end());
    float maxRipRadius = sqrt(pow(window_width,2)+pow(window_height,2));
////////////////////////////////////////////////////////////////////////////////////////
	// Start the game loop
    while (window.isOpen())
    {
    mute.muteSet(gamemusic);
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            //P Pressed: Pause
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                {pausefunc(window_width, window_height, mute, window, gamemusic, allPegs, testChip, binMoney,
                scoreboard, rightPlinkoDoor, brita, chipText, moneyText, ripples);}

            //Mouse moved
            if(event.type == sf::Event::MouseMoved && counter == 0)
                testChip.setPosition(event.mouseMove.x, event.mouseMove.y);

            //Mouse pressed
            if(event.type == sf::Event::MouseButtonPressed && testChip.getPosition().y >= 20 && counter == 0)
                {
                chipError.setString("Chip Must Be Dropped\n \n    at top of Board");
				gamebeep.play();
				chipError.setPosition(window_width/4.5, window_height/4.2);
				}
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
        //delete ripples that are out of window -- saves comput
        for(int i = 0; i<ripples.size();i++)
            {ripples[i].updateRipple();
            if(ripples[i].getRadius()>maxRipRadius)
                ripples.erase(ripples.begin()+i);}

        window.clear(sf::Color::White);

        // Draw the sprite
        for (int i=0; i<allPegs.size(); i++)
            {window.draw(allPegs[i]);}
        window.draw(testChip);
        window.draw(chipError);

        for(int i = 0; i<9; i++)
            window.draw(binMoney[i]);

        window.draw(scoreboard);
        window.draw(brita);
        window.draw(mop);
        window.draw(leftPlinkoDoor);
        window.draw(rightPlinkoDoor);
        window.draw(chipText);
        window.draw(moneyText);
        if(round == 1 && leftPlinkoDoor.getPosition().x>-180)
            {leftPlinkoDoor.move(-5,0);}
        if(round == 2 && leftPlinkoDoor.getPosition().x ==-180 && rightPlinkoDoor.getPosition().x<1000)
            {rightPlinkoDoor.move(5,0);
            rightPlinkoDoor.rotate(20);}
        for(int i = 0; i<ripples.size();i++)
            {window.draw(ripples[i]);}

        // Update the window
        window.display();
        //set frame rate
        window.setFramerateLimit(FRAME_RATE);
////////////////////////////////////////////////////////////////////////////////////////

    //resetting necessary parameters for the next chip to be able to drop properly
        if(mouseWasPressed)
        {
            counter=1;
            isInBin=0;
            finishedDropping=0;
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
                        {
                            MomentumTransfer(testChip,allPegs[i]);
                            //Generate ripple for board pegs
                            if( i >= 350 && i<=allPegs.size())
                            {
                                float radVel = 5;
                                Ripple rip(allPegs[i].getFillColor(), allPegs[i].getPosition().x,
                                            allPegs[i].getPosition().y, allPegs[i].getRadius(), radVel);
                                ripples.push_back(rip);
                                collisionSound.play();
                            }
                        }
            }

            //update position of Chip
            testChip.setNextPosition();
            testChip.setPosition(testChip.getNextPosition().x,testChip.getNextPosition().y);
            testChip.applyGravity();

            //for if the chip reaches a bin:
            sf::Vector2f position = testChip.getPosition();

            //if the chip lands on a bottom peg, then, it has "finished dropping"
            for(int i=0; i<250; i++)
            {
                if(getDistance(testChip.getNextPosition(),allPegs[i].getPosition())<=
                                (testChip.getRadius()+allPegs[i].getRadius()))
                {
                    for (int j=250; j<350; j++)
                    {
                        if (getDistance(testChip.getNextPosition(),allPegs[j].getPosition())<=
                                (testChip.getRadius()+allPegs[j].getRadius()))
                            {finishedDropping=0;}

                        else
                            finishedDropping=1;
                    }
                }
            }

            //checking for valid x position (aka valid position for landing in a bin)
            bool hasValidXPosition=0;
            hasValidXPosition=checkForValidBinPosition(testChip);


            //if the chip has finished dropping and has a valid x position,
            //then set its velocity to 0 and place it so that it fits nicely in the bin
            if (finishedDropping && hasValidXPosition)
            {
                testChip.setVelocity(0,0);
                testChip.setPosition(position.x,505+0.5-allPegs[0].getRadius()-testChip.getRadius());
            }
        //This takes care of the case for if the chip has not landed in a bin, or
        //if it hasn't finished dropping.
            else
            {
                for (int j=0; j<350; j++)
                {
                    if (getDistance(testChip.getNextPosition(),allPegs[j].getPosition())<=
                            (testChip.getRadius()+allPegs[j].getRadius()))
                    {
                        while (!hasValidXPosition)
                        {
                            //do momentum transfer and update position of the chip
                            MomentumTransfer(testChip,allPegs[j]);
                            hasValidXPosition=checkForValidBinPosition(testChip);
                            testChip.setNextPosition();
                            testChip.setPosition(testChip.getNextPosition().x,testChip.getNextPosition().y);
                            testChip.applyGravity();

                            //redrawing the updated position for smoothness
                            window.clear(sf::Color::White);
                            for (int i=0; i<allPegs.size(); i++)
                                    {window.draw(allPegs[i]);}
                            window.draw(testChip);
                            window.draw(chipError);
                            for(int i = 0; i<9; i++)
                                window.draw(binMoney[i]);
                            window.draw(scoreboard);
                            window.draw(chipText);
                            window.draw(moneyText);
                            window.draw(brita);
                            window.draw(mop);
                            window.draw(leftPlinkoDoor);
                            window.draw(rightPlinkoDoor);
                            for(int i = 0; i<ripples.size();i++)
                                {window.draw(ripples[i]);}
//                             Update the window
                            window.display();
                            window.setFramerateLimit(FRAME_RATE);
                        }
                    }
                }
            }

            //initializing
            double binwidth=55.5;
            int money=0;

        //This section determines the amount of money awarded depending on the bin.
            if(position.y>=485 && finishedDropping)
            {
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
                round++;
            }

            //initializing bounceCount vector
//            if (colorCounter==0)
//                bounceCount.resize(allPegs.size(),0);
//                //Kept the index of i the same by making the vector to include all pegs.
//                //Note: only the board pegs will be lit up.
//
//            //changing board pegs that have a collision with the chip
//            for (int i=350; i<allPegs.size(); i++)
//            {
//                if(getDistance(testChip.getNextPosition(),allPegs[i].getPosition())<=
//                        (testChip.getRadius()+allPegs[i].getRadius()))
//                {
//                    //make boing sound
//                    collisionSound.play();
//
//                    //change color of peg depending on number of bounces on same peg
//                    //have it rotate through same few colors (total: 5)
//                    if (bounceCount[i]%5==0)
//                        allPegs[i].setFillColor(sf::Color::Magenta);
//                    else if (bounceCount[i]%5==1)
//                        allPegs[i].setFillColor(sf::Color::Red);
//                    else if (bounceCount[i]%5==2)
//                        allPegs[i].setFillColor(sf::Color::Green);
//                    else if (bounceCount[i]%5==3)
//                        allPegs[i].setFillColor(sf::Color::Blue);
//                    else
//                        allPegs[i].setFillColor(sf::Color::Cyan);
//
//                    bounceCount[i]++;
//                    colorCounter++;
//                }
//            }

        //if the chip has reached a bin
            if(isInBin)
            {
                //This if statement occurs when you run out of chips.
                if (chipCount==maxNumberOfChips)
                {
                    chipCanDrop=0;
                    gamemusic.stop();
                    winfunc(window_width, window_height, mute, window, c, totalMoney, allPegs);
                }
                chipError.setColor(sf::Color::Blue);
                counter=0;
                mouseWasPressed=0;
                chipError.setString("Move Mouse to Top of Board\n \n       to Drop Next Chip");
                chipError.setPosition(window_width/7.5, window_height/4.2);
                //Setting the velocity to 0 is a failsafe for if the chip lands incorrectly.
                //This will prevent the next chip from being affected.
                testChip.setVelocity(0,0);
                selectionMusic.play();
            }
        }
    }
}

void checkForUniquePosition (vector<Peg> &pegVec) //need to pass vector by reference
{
    int pegSize = pegVec.size();


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
            //==the chip diameter + 3*the peg diameter

            //if a peg is to be created too close to a different peg, then reset this process
            if(distance<=collisionDistance && i!=j)
            {
                pegVec[i].setRandomPosition();
                j=-1; // because j++ before it repeats through the loop
            }
        }
    }
}

vector<Peg> setupOriginalBoard()
{
            vector<Peg> allPegs;

            const int pegsPerLineOdd=9;
            const int pegsPerLineEven=10;

            //creating oddPegs vector
            vector<Peg> oddPegs;
            for (int i=0; i<36; i++)
                {oddPegs.push_back(Peg(true));}
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
                {evenPegs.push_back(Peg(true));}
            int evenSIZE=evenPegs.size();
            int evenLine=0;
            for (int i=0; i<evenSIZE; i++)
            {
                int offset=25;
                if (i%pegsPerLineEven==0 && i!=0)
                    {evenLine++;}
                evenPegs[i].setPosition(offset+(i%pegsPerLineEven)*50,100+100*evenLine);
            }
            allPegs.insert(allPegs.end(),evenPegs.begin(),evenPegs.end());
            allPegs.insert(allPegs.end(),oddPegs.begin(),oddPegs.end());
            return allPegs;
}
vector<Peg> setupRandomBoard()
{
    //Note: 65 pegs is the max number that can be realized in a reliable manner.
            vector<Peg> pegVec;
            for (int i=0; i<65; i++)
            {
                pegVec.push_back(Peg(true));
                pegVec[i].setRandomPosition();
            }
            checkForUniquePosition(pegVec);
            return pegVec;
}

vector<Peg> setupBinPegs()
{
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

            return binPegs;
}

vector<Peg> setupBottomPegs()
{
            vector<Peg> bottomPegs;
            for (int i=0; i<250; i++)
            {
                bottomPegs.push_back(Peg());
                bottomPegs[i].setPosition(i*2,505);
            }
            return bottomPegs;
}

bool checkForValidBinPosition(PlinkoChip testChip)
{
    sf::Vector2f position = testChip.getPosition();
    double radius=testChip.getRadius();

    double binwidth=55.5;

    bool validXposition=0;

    if (position.x>=0+radius && position.x<=binwidth-radius)
        validXposition=1;
    else if (position.x>=binwidth+radius && position.x<=2*binwidth-radius)
        validXposition=1;
    else if (position.x>=2*binwidth+radius && position.x<=3*binwidth-radius)
        validXposition=1;
    else if (position.x>=3*binwidth+radius && position.x<=4*binwidth-radius)
        validXposition=1;
    else if (position.x>=4*binwidth+radius && position.x<=5*binwidth-radius)
        validXposition=1;
    else if (position.x>=5*binwidth+radius && position.x<=6*binwidth-radius)
        validXposition=1;
    else if (position.x>=6*binwidth+radius && position.x<=7*binwidth-radius)
        validXposition=1;
    else if (position.x>=7*binwidth+radius && position.x<=8*binwidth-radius)
        validXposition=1;
    else if (position.x>=8*binwidth+radius && position.x<=9*binwidth-radius)
        validXposition=1;
    else
        validXposition=0;

    return validXposition;
}
