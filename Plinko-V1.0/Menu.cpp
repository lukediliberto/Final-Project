#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include "Mute.h"

using namespace std;

int FRAME_RATE = 10;

//Menu constructor
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

//draw menu on the window passed
void Menu::draw(sf::RenderWindow &window)
{
    for(int i = 0; i < NUM_ITEMS; i++)
        window.draw(menuText[i]);
}

//change selected item when keyboard pressed up
void Menu::MoveUp()
{
    if(selectedIndex > 0)
    {
        menuText[selectedIndex].setColor(sf::Color::Black);
        selectedIndex--;
        menuText[selectedIndex].setColor(textcolor);
    }
}

//change selected item when keyboard pressed down
void Menu::MoveDown()
{
    if(selectedIndex < NUM_ITEMS - 1)
    {
        menuText[selectedIndex].setColor(sf::Color::Black);
        selectedIndex++;
        menuText[selectedIndex].setColor(textcolor);
    }
}
//return index of selected item
int Menu::GetSelected()
{
    return selectedIndex;
}
//set strings for menu options
void Menu::SetMenuItems(int index, string text)
{
    menuText[index].setString(text);
}

//set number of menu options
void Menu::SetNumItems(int num)
{
    NUM_ITEMS = num;
}
//return number of menu options
int Menu::GetNumItems()
{
    return NUM_ITEMS;
}
//set position of menu items
void Menu::SetTextPosition(int index, float x, float y)
{
    menuText[index].setPosition(sf::Vector2f(x,y));
}
//set color of menu items
void Menu::SetTextColor(sf::Color color)
{
    textcolor = color;
}

//MAIN MENU
void menufunc(float width, float height, Mute &mute)
{
    vector<string> menuitems;
    menuitems.push_back("Play");
    menuitems.push_back("Quit");
    menuitems.push_back(mute.getText());

    // Create the main window
    sf::RenderWindow menuwindow(sf::VideoMode(width, height), "It's Menu Time!");
    menuwindow.setFramerateLimit(FRAME_RATE);
    menuwindow.setPosition(sf::Vector2i(0,0));

    Menu menu(width, height, menuitems.size());

    //set menu options
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
    menuBuffer.loadFromFile("plinkomenu.wav");
    selectionBuffer.loadFromFile("selection.wav");
    sf::Sound menuMusic, selectionMusic;
    menuMusic.setBuffer(menuBuffer);
    menuMusic.play();
    menuMusic.setLoop(true);
    selectionMusic.setBuffer(selectionBuffer);

	// Start the menu loop
    while (menuwindow.isOpen())
    {
    mute.muteSet(menuMusic);
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
                    //Process menu selections
                    case sf::Keyboard::Return:
                        switch(menu.GetSelected())
                        {
                        //open sub menu
                        case 0:
                            menuMusic.stop();
                            selectionMusic.play();
                            subfunc(width, height, mute, menuwindow, menu);
                            break;
                        //quit
                        case 1:
                            menuMusic.stop();
                            menuwindow.close();
                            break;
                        case 2:
                            mute.flip();
                            mute.muteSet(menuMusic);
                            menu.SetMenuItems(2,mute.getText());
                            break;
                        }
                }
            break;
            }
        }
        // Clear screen
        menuwindow.clear(sf::Color::White);
        //Draw main menu and background
        menuwindow.draw(sprite);
        menu.draw(menuwindow);
        // Update the window
        menuwindow.display();
    }

}

void subfunc(float width, float height, Mute &mute, sf::RenderWindow &submenuwindow, Menu menu)
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

    int mode = -1;
	// Start the sub menu loop
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
                    //Process submenu selections
                    case sf::Keyboard::Return:
                        switch(submenu.GetSelected())
                        {
                        //original mode
                        case 0:
                            submenuwindow.close();
                            gamefunc('O', mute);
                            break;
                        //random mode
                        case 1:
                            submenuwindow.close();
                            gamefunc('R', mute);
                            break;
                        //back to main menu
                        case 2:
                            mode = 0;
                            break;
                        }
                }
            break;
            }
        }
        //if back selected, function exits
        if(mode == 0)
            break;
        //Draw sub menu
        submenu.draw(submenuwindow);
        // Update the window
        submenuwindow.display();
    }

}

//PAUSE MENU
void pausefunc(float width, float height, Mute &mute, sf::RenderWindow &pausewindow, sf::Sound &gamemusic,
                vector <Peg> allPegs, PlinkoChip testChip, sf::Text binMoney[], sf::Sprite scoreboard,
                sf::Sprite rightPlinkoDoor,sf::Sprite brita, sf::Text chipText, sf::Text moneyText)
{
    vector<string> pauseitems;
    pauseitems.push_back("Resume");
    pauseitems.push_back("Main Menu");
    pauseitems.push_back(mute.getText());
    pauseitems.push_back("Quit");

    Menu pausemenu(width, height, pauseitems.size(), sf::Color::Magenta, 24);
//Menu::Menu(float width, float height, int num, sf::Color color = sf::Color::Magenta, unsigned int textsize = 30)
    for(int i = 0; i<pauseitems.size(); i++)
        {pausemenu.SetMenuItems(i, pauseitems[i]);};


    int mode = -1;
	// Start the pause loop
    while (pausewindow.isOpen())
    {
    pausewindow.clear(sf::Color::White);

        // Process events
        sf::Event event;
        while (pausewindow.pollEvent(event))
        {
            switch(event.type)
            {
            // Close window : exit
            case sf::Event::Closed:
                gamemusic.stop();
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
                    //Process pause menu selections
                    case sf::Keyboard::Return:
                        switch(pausemenu.GetSelected())
                        {
                        //return to game
                        case 0:
                            mode = 0;
                            break;
                        //return to main menu
                        case 1:
                            gamemusic.stop();
                            pausewindow.close();
                            menufunc(500, 500, mute);
                            break;
                        //Mute
                        case 2:
                            mute.flip();
                            mute.muteSet(gamemusic);
                            pausemenu.SetMenuItems(2,mute.getText());
                            break;
                        //quit
                        case 3:
                            gamemusic.stop();
                            pausewindow.close();
                            break;
                        }
                }
            break;
            }
        }
        //if resume game selected, function exits
        if(mode == 0)
            break;
        //draw pause menu
        pausemenu.draw(pausewindow);

        for (int i=0; i<allPegs.size(); i++)
            {pausewindow.draw(allPegs[i]);}
        pausewindow.draw(testChip);
        for(int i = 0; i<9; i++)
            pausewindow.draw(binMoney[i]);
        pausewindow.draw(scoreboard);
        pausewindow.draw(brita);
        pausewindow.draw(rightPlinkoDoor);
        pausewindow.draw(chipText);
        pausewindow.draw(moneyText);
        pausewindow.display();
    }

}

//structure and operator for use in winfunc leaderboard
struct userscore{string name; int score;};
bool byScoreDesc(const userscore &a, const userscore &b){return a.score>=b.score;}
vector<userscore> getLeaderBoard(char c);
int getUserRank(vector<userscore> leaderboard, int score);
void writeLeaderBoardFile(char c, const vector<userscore> &leaderboard);

void winfunc(float width, float height, Mute &mute, sf::RenderWindow &winwindow, char c, int cash, vector<Peg> allPegs)
{
    //end of game music
    sf::Sound winmusic;
    mute.muteSet(winmusic);
    sf::SoundBuffer winbuffer;
    if(cash > 0)
        winbuffer.loadFromFile("winmusic.wav");
    if(cash == 0)
        winbuffer.loadFromFile("sadmusic.wav");
    winmusic.setBuffer(winbuffer);
    winmusic.setLoop(true);
    winmusic.play();

    sf::Font font;
    if(!font.loadFromFile("PLINKO2K.TTF"))
        cout<<"Error loading font (WinFunc)"<<endl;
    //set textures
    sf::Texture scoreboardTexture;
    scoreboardTexture.loadFromFile("scoreboard.png"); //put possible failure statement here
    sf::Sprite scoreboard(scoreboardTexture);
    scoreboard.setPosition(175,510);
    sf::Text chipText, moneyText;

    chipText.setFont(font);
    chipText.setCharacterSize(24);
    chipText.setColor(sf::Color::Black);
    chipText.setString(to_string(0));
    chipText.setPosition(283,623);

    moneyText.setFont(font);
    moneyText.setCharacterSize(24);
    moneyText.setColor(sf::Color::Black);
    moneyText.setPosition(195,535);

    sf::Texture rightPlinkoDoorTexture;
    rightPlinkoDoorTexture.loadFromFile("rightplinkodoor.png");
    sf::Sprite rightPlinkoDoor(rightPlinkoDoorTexture);
    rightPlinkoDoor.setPosition(175+150+10,513);
    rightPlinkoDoor.setScale(.185,.17);

    sf::Texture leftPlinkoDoorTexture;
    leftPlinkoDoorTexture.loadFromFile("leftplinkodoor.png");
    sf::Sprite leftPlinkoDoor(leftPlinkoDoorTexture);
    leftPlinkoDoor.setPosition(5,513);
    leftPlinkoDoor.setScale(.185,.17);

    vector<string> winitems;
    winitems.push_back("Play Again");
    winitems.push_back("Main Menu");
    winitems.push_back("Quit");
    winitems.push_back("Clear High \n  Scores");

    Menu winmenu(width, height, winitems.size(), sf::Color::Green, 22);

    for(int i = 0; i<winitems.size(); i++)
        {winmenu.SetMenuItems(i, winitems[i]);}

    sf::Text finalDialogue;

    //return highscores for game mode
    vector <userscore> leaderboard = getLeaderBoard(c);
    bool awaitNameEntry = false;
    //determine user rank
    int userRank = getUserRank(leaderboard,cash);
    userscore current;
    current.name = "";
    current.score = cash;
    leaderboard.push_back(current);
    std::sort(leaderboard.begin(), leaderboard.end(),byScoreDesc);
    string money=to_string(cash);
    moneyText.setString("$"+money);

    if(cash >= leaderboard[9].score)
        {money = "Congratulations!!!\n You Are #"+ to_string(userRank+1) + "\n Enter Your Name";
        awaitNameEntry = true;}
    else if(cash != 0)
        money="Congratulations!!!\n\n   You win: $"+ money;
    else if(cash == 0)
        money="You win $0\nBetter Luck Next Time";
    finalDialogue.setFont(font);
    finalDialogue.setCharacterSize(24);
    finalDialogue.setColor(sf::Color::Green);
    finalDialogue.setString(money);
    finalDialogue.setPosition(width/4, height/8);

    //string line;
    sf::Text high("HIGHSCORES", font, 30);
    high.setPosition(width/3.15, height/3);
    high.setColor(sf::Color::Black);

    sf::Text scores[10];
        for(int i = 0; i<10; i++)
        {
            string line;
            line = to_string(i+1) + ". "+ leaderboard[i].name + " $" + to_string(leaderboard[i].score);
            scores[i].setString(line);
            scores[i].setFont(font);
            scores[i].setCharacterSize(20);
            scores[i].setColor(sf::Color::Black);
            if(i == userRank)
            scores[i].setColor(sf::Color::Red);
            if(i<5)
                scores[i].setPosition(width/26, height/4 +100 +i*20);
            else
                scores[i].setPosition(width/2, height/4 + i*20);
        }

    int counter = 5;
	// Start the game loop
//for name entry
std::string userstr;
int usrcount = 0;

    while (winwindow.isOpen())
    {
        // Process events
        sf::Event event;
        if(awaitNameEntry)
        {
            while (winwindow.pollEvent(event))
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                        winwindow.close();
                    //Process name entered
                    case sf::Event::TextEntered:
                        if (isalnum(event.text.unicode)&&usrcount <7 )
                        {
                            userstr += static_cast<char>(event.text.unicode);
                            leaderboard[userRank].name = userstr;
                            usrcount++;
                        }
                    case sf::Event::KeyReleased:
                        switch(event.key.code)
                        {
                            case sf::Keyboard::Return:
                                awaitNameEntry = false;
                                if(userstr.empty())
                                    {leaderboard[userRank].name = "-";}
                                writeLeaderBoardFile(c, leaderboard);
                                break;
                            case sf::Keyboard::BackSpace:
                                    try
                                    {userstr.erase(userstr.size() - 1);
                                    leaderboard[userRank].name = userstr;
                                    usrcount--;}
                                    catch(out_of_range)
                                    {break;}
                                break;
                        }
                }
            }
        }
        //After name entered, open final menu
        if(!awaitNameEntry)
        {
            while (winwindow.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                    winwindow.close();
                    winmusic.stop();
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
                            //Play again
                            case 0:
                                winmusic.stop();
                                winwindow.close();
                                gamefunc(c, mute);
                                break;
                            //main menu
                            case 1:
                                winmusic.stop();
                                winwindow.close();
                                menufunc(500, 500, mute);
                                break;
                            //quit
                            case 2:
                                winmusic.stop();
                                winwindow.close();
                                break;
                            //clear highscores
                            case 3:
                                for(int i = 0; i<10; i++)
                                    {leaderboard[i].name = "-";
                                    leaderboard[i].score = 0;}
                                for(int i = 0; i<10; i++)
                                    {string line;
                                    line = to_string(i+1) + ". "+ leaderboard[i].name +
                                    " $" + to_string(leaderboard[i].score);
                                    scores[i].setString(line);}
                                writeLeaderBoardFile(c, leaderboard);
                                break;
                            }
                    }
                break;
                }
            }
        }
//NOTE: Leaderboard requires an alternastive to usleep strategy for winning color peg animation.
//Clearing window allows for the Leaderboard to be update in screen but prevents the color changing pegs.
////////////////////////////////////////////////////////////////////////////////////////////
//BLINK RATE METHOD BEGIN
    //alternate colors of board pegs
    int blinkrate = 30;
    if(cash > 0)
    {
        for(int i = 0; i<allPegs.size(); i++)
        {
            if(counter % blinkrate == 0 && i % 2 == 0)
            {
                allPegs[i].setFillColor(sf::Color::Cyan);
                finalDialogue.setColor(sf::Color::Green);
            }
            if(counter % blinkrate == 0 && i % 2 == 1)
            {
                allPegs[i].setFillColor(sf::Color::Blue);
                finalDialogue.setColor(sf::Color::Green);
            }
            if(counter % blinkrate == blinkrate/2 && i % 2 == 0)
            {
                 allPegs[i].setFillColor(sf::Color::Blue);
                 finalDialogue.setColor(sf::Color::Yellow);
            }
            if(counter % blinkrate == blinkrate/2 && i % 2 == 1)
            {
                allPegs[i].setFillColor(sf::Color::Cyan);
                finalDialogue.setColor(sf::Color::Yellow);
            }
       }
    }
        winwindow.clear(sf::Color::White);
        for(int i=0; i<allPegs.size();i++)
            {winwindow.draw(allPegs[i]);}
//BLINK RATE METHOD END
////////////////////////////////////////////////////////////////////////////////////////////
//SLEEP METHOD BEGIN
 /*for(int i = 0; i<allPegs.size(); i++)
        {
            //sleep and change pegs, un-comment usleep below to test
            if(counter % 2 == 0 && i % 2 == 0)
            {
                allPegs[i].setFillColor(sf::Color::Cyan);
                finalDialogue.setColor(sf::Color::Green);
            }
            if(counter % 2 == 0 && i % 2 == 1)
            {
                allPegs[i].setFillColor(sf::Color::Blue);
                finalDialogue.setColor(sf::Color::Green);
            }
            if(counter % 2 == 1 && i % 2 == 0)
            {
                 allPegs[i].setFillColor(sf::Color::Blue);
                 finalDialogue.setColor(sf::Color::Yellow);
            }
            if(counter % 2 == 1 && i % 2 == 1)
            {
                allPegs[i].setFillColor(sf::Color::Cyan);
                finalDialogue.setColor(sf::Color::Yellow);
            }
            winwindow.draw(allPegs[i]);
       }
        usleep(200000);*/
winwindow.draw(scoreboard);
winwindow.draw(moneyText);
winwindow.draw(chipText);
winwindow.draw(rightPlinkoDoor);
//SLEEP METHOD END
////////////////////////////////////////////////////////////////////////////////////////////
        counter++;
////////////////////////////////////////////////////////////////////////////////////////////
    if(awaitNameEntry)
        {string updateline;
        updateline = to_string(userRank+1) + ". "+ leaderboard[userRank].name + " $"+ to_string(leaderboard[userRank].score);
        scores[userRank].setString(updateline);}

        winwindow.draw(finalDialogue);
        winwindow.draw(high);
        for(int i = 0; i<10; i++)
            {winwindow.draw(scores[i]);}
        if(!awaitNameEntry)
            {winmenu.draw(winwindow);}
        if(awaitNameEntry)
            {winwindow.draw(leftPlinkoDoor);}
        winwindow.display();
    }
    winmusic.stop();
}

//Load the leaderboard and sort and return the scores
vector<userscore> getLeaderBoard(char c)
    {
        ifstream infile;
        if(c == 'O')
            {infile.open("highscoresOrig.txt");}
        if (c == 'R')
            {infile.open("highscoresRand.txt");}
        vector<userscore> leaderboard;
        int i = 0;
        while (infile.good())
            {
                userscore temp;
                infile >> temp.name;
                infile >> temp.score;
                infile.ignore(1);
                leaderboard.push_back(temp);
                i++;
            }
        infile.close();
        sort(leaderboard.begin(), leaderboard.end(),byScoreDesc);
        for(int i = 0; i<10; i++)

        return leaderboard;
    }
//write updates to highscores file
void writeLeaderBoardFile(char c, const vector<userscore> &leaderboard)
    {
        ofstream outFile;

        if(c == 'O')
            {outFile.open("highscoresOrig.txt");}
        if (c == 'R')
            {outFile.open("highscoresRand.txt");}
        for(int i = 0; i<10; i++)
            {outFile << leaderboard[i].name << " " << leaderboard[i].score << "\n";}
        outFile.close();
    }
//determine user rank in current highscores
int getUserRank(vector<userscore> leaderboard, int score)
    {
    for(int i = 0; i<9; i++)
        {
            if (score >= leaderboard[i].score)
            return i;
        }
    }
