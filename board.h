/*
 * Project: minesweeper
 * Name: board.h
 * Author: Tygan Chin
 * Purpose: Interface for the minesweeper board class. Creates a Board object of the given rows and cols for the minesweeper game.
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <set>
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

class Board
{
public:

    void setBoard(int numRows, int numCols, int numBombs);
    int move(int row, int col);
    bool placeFlag(int row, int col);
    bool isFlag(int row, int col);
    bool isShown(int row, int col);
    int numFlags();
    int getNumRows();
    int getNumCols();
    int getNumber(int row, int col);
    bool won();
    pair<int, int> getBombPos();

private:

    /* represents a single space on the board */
    struct Square {
        int val;
        bool shown;
        bool flag;
    };

    vector<vector<Square> > mineField;
    set<pair<int, int> > bombLocs;

    int spacesLeft;
    int numRows;
    int numCols;
    float titleScale;
    size_t bombs;
    int flags;
    bool firstMove;

    /* helper functions */
    void openSpace(int &row, int &col);
    void setBombs(int row, int col);
    void setNumbers(int &row, int &col);


    //test funcs
    void testprint();

};

#endif
