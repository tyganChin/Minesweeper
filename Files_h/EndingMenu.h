/*
 * Project: minesweeper
 * Name: EndingMenu.h
 * Author: Tygan Chin
 * Purpose: Interface for the EndingMenu class. Declares the functions that
 *          display the ending menu for the user which includes a win/loss
 *          message, the difficulty of the game, the number of wins on the
 *          difficulty, the time elasped, the best time recorded, and quit
 *          and continue buttons.
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
    SFMLhelper SFML;
    Sound music, soundEffect, button;
    Sprite backgroundImage;
    textBox winNumber, difficultyInitial, winLoss, bestTime, time, quit, cont;

    /* helper functions */
    bool decision(RenderWindow &window);
    void draw(RenderWindow &window);
    void playSound();

    /* image functions */
    void setSounds(bool &gameWon);
    void setBackgroundImage(bool &gameWon);
    void setWinNumber(bool &gameWon, int &difficulty);
    void setDifficulty(int &difficulty);
    void setWinLoss(bool &gameWon);
    void setTimes(bool &gameWon, Time &gameTime, int &difficulty);
    bool setBestTime(bool &gameWon, Time &gameTime, string file);

    /* image helper funcs */
    string getWins(bool &gameWon, string filename);
};

#endif
