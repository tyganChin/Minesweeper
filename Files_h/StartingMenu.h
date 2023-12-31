/*
 * Project: minesweeper
 * Name: StartingMenu.h
 * Author: Tygan Chin
 * Purpose: Interface for the Starting menu for the minesweeper game. Defines 
 *          the functions that display the game menu for the minesweeper game 
 *          and gets the desired difficulty of the user.
 */

#ifndef STARTINGMENU_H
#define STARTINGMENU_H

#include "SFML/Graphics.hpp"
#include "SFMLhelper.h"
#include <string>

using namespace sf;
using namespace std;

class StartingMenu 
{
public: 

    /* constructor */
    StartingMenu();
    
    /* run function */
    int getDifficulty();

private:

    /* member variables */
    int difficulty;
    SFMLhelper SFML;
    Sound music, button;
    textBox menuTitle;
    vector<textBox> diffButtons;

    /* helper funcs */
    bool difficultyChosen(RenderWindow &window);
    void draw(RenderWindow &window);
    void playButton();
};

#endif
