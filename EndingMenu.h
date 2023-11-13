/*
 * Project: minesweeper
 * Name: EndingMenu.h
 * Author: Tygan Chin
 * Purpose: 
 */

#ifndef ENDINGMENU_H
#define ENDINGMENU_H

#include "SFML/Graphics.hpp"
#include "SFMLhelper.h"
#include <string>

using namespace sf;
using namespace std;

class EndingMenu
{
public:
    
    /* constructor */
    EndingMenu(bool gameWon, Time time, int &difficulty);

    /* run function */
    bool playAgain();

private:

    /* member variables */
    bool again;
    Sound music, soundEffect, button;
    Sprite backgroundImage;
    textBox winNumber, difficultyInitial, winLoss, bestTime, time, quit, cont;

    /* helper functions */
    bool decision(RenderWindow &window);
    void draw(RenderWindow &window);
    void playSound();
    string getBestTime(string filename);
    void updateBest(bool &gameWon, Time &gameTime, int &difficulty, int oldTime);

    /* image functions */
    void setSounds(bool &gameWon);
    void setBackgroundImage(bool &gameWon);
    void setWinNumber(bool &gameWon, int &difficulty);
    void setDifficulty(int &difficulty);
    void setWinLoss(bool &gameWon);
    void setTimes(bool &gameWon, Time &gameTime, int &difficulty);
    void setQuitContinue();

    /* image helper funcs */
    bool setBestTime(bool &gameWon, Time &gameTime, string file);
    string getWins(bool &gameWon, string filename);
};

#endif
