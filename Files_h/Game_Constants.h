/*
 * Project: minesweeper
 * Name: Game_Constants.h
 * Author: Tygan Chin
 * Purpose: Defines the constants used in the entire Minesweeper program
 * Note: The Menu and Game widths should be adjusted to fit the screen
 *       size of the computer the minesweeper program will run on.
 */

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <string>
#include "SFMLhelper.h"

using namespace std;

/* 
 * Dimensions of the menus and game render windows. Should be adjuseted to fit
 * within the screen size of the computer running the program. All other images
 * should scale with these two numbers.
 */
static const int MENU_WIDTH = 900;
static const int GAME_WIDTH = 800;

/* number of difficulties in the game */
static const int NUM_DIFFS  = 3;

/* move outcomes */
static const int NUMBERED_SPACE =  2;
static const int OPEN_SPACE     =  1;
static const int MINE_HIT       = -1;
static const int NO_SPACE       =  0;

/* mine value */
static const int MINE = -1;

/* button sound info */
static const soundInfo BUTTON = {"Sounds/button.mp3", 100, 1.0, false};

#endif
