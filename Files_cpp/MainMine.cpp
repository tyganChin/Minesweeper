/*
 * Project: minesweeper
 * Name: MainMine.cpp
 * Author: Tygan Chin
 * Purpose: Driver of the minesweeper game. Runs and reruns the game until the
 *          the user quits.
 */

#include "../Files_h/Minesweeper.h"

/* 
 * main
 * purpose: Run and rerun the minesweeper game until the user quits or exits
 *          the game
 * parameters: n/a
 * returns: 1
 */
int main(void)
{
    bool play = true;
    while (play) {
        Minesweeper mineGame;
        play = mineGame.run();        
    }
    return 1;
}
