/*
 * Project: minesweeper
 * Name: Board.h
 * Author: Tygan Chin
 * Purpose: Interface for the minesweeper board class. Defines functions that 
 *          create a Board object for the a minesweeper game using the given 
 *          rows, columns, and number of bombs.
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <set>
#include <random>
#include "Game_Constants.h"
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

/* the coordinate type for the board (row:col) */ 
typedef pair<int, int> Pos;

class Board
{
public:
    
    /* constructor */
    Board();

    /* initializer (must be called in order for class to operate correctly) */
    void setBoard(int numRows, int numCols, int numBombs);

    /* game moves */
    int move(int row, int col);
    bool placeFlag(int row, int col);

    /* getters */
    bool won();
    bool isFlag(int row, int col);
    bool isShown(int row, int col);
    int numFlags();
    int getNumRows();
    int getNumCols();
    int getNumber(int row, int col);
    bool revealMine();

private:

    /* represents a single space on the board */
    struct Cell {
        int val;
        bool shown;
        bool flag;
    };

    /* the board representation and set of bomb locations */
    vector<vector<Cell> > mineField;
    set<Pos> bombLocs;

    /* member variables */
    int spacesLeft;
    int numRows;
    int numCols;
    size_t bombs;
    int flags;
    bool firstMove;
    mt19937 gen;
    Pos mineHit;

    /* helper functions */
    void openSpace(int &row, int &col);
    void setBombs(int row, int col);
    void setNumbers(int &bombRow, int &bombCol);

    /* test function */
    void testprint();
};

#endif
