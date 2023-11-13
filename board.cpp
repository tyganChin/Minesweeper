/*
 * Project: minesweeper
 * Name: 
 * Author: Tygan Chin
 * Purpose: 
 */

#include "board.h"
#include "SFMLhelper.h"
#include <random>
#include <set>
#include <iostream>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <random>

/* move outcomes */
static const int NUMBERED_SPACE = 2;
static const int OPEN_SPACE     = 1;
static const int NO_SPACE       = 0;
static const int MINE           = -1;

/* adjacent block iterators */
static const int ADJACENT_BLOCKS = 8;
static const int R[] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int C[] = {-1, 0, 1, -1, 1, -1, 0, 1};

/* invalid pair for when user has gotten all of the bombs from the bombLOc set */
static const pair<int, int> OUT_OF_BOMBS = make_pair(-1, -1);

/* 
 * setBoard
 * purpose: Initialize a board of the given size
 * arguments: Number of rows, columns, and bombs
 * returns: n/a
 */
void Board::setBoard(int colNum, int rowNum, int numBombs)
{
    /* set memeber variables */
    numRows = rowNum;
    numCols = colNum;
    bombs = flags = numBombs;
    spacesLeft = (numCols * numRows) - numBombs;
    firstMove = true;

    /* initialize empty board */
    for (int i = 0; i < numRows; ++i) {
        mineField.push_back(vector<Square>());
        for (int j = 0; j < numCols; ++j) {
            mineField[i].push_back((Square){0, false, false});
        }
    }
}

/*
 * move
 * arguments: Coordinates of user move 
 * purpose: Executes the users move
 * returns: False if a mine was hit, true otherwise
 */ 
int Board::move(int row, int col)
{
    /* set bombs and numbers if user first move */
    if (firstMove) {
        setBombs(row, col);    
        firstMove = false;
    }

    /* add flag back to total number if necessary */
    if (mineField[row][col].flag) {
        ++flags;        
    }

    /* check if square is already revealed  */
    if (mineField[row][col].shown) {
        return NO_SPACE;        
    }
    
    /* check if mine was hit */
    if (mineField[row][col].val == -1) {
        mineField[row][col].shown = true;
        bombLocs.erase(make_pair(row, col));
        return MINE;
    } 

    /* reveal cell and decrement spaces left */
    mineField[row][col].shown = true;
    --spacesLeft;

    /* reveal adjacent spaces if open space */
    if (mineField[row][col].val == 0) {
        openSpace(row, col);        
        return OPEN_SPACE; 
    }

    return NUMBERED_SPACE;            
}

/*
 * openSpace
 * arguments: Coordinates of move
 * purpose: Reveal the surrounding spaces of the chosen open space on the board
 * returns: n/a
 */
void Board::openSpace(int &row, int &col)
{
    /* reveal the blocks in the direct vicinty of the open space */
    for (int i = 0; i < ADJACENT_BLOCKS; ++i) {

        /* reveal space if in bounds */
        bool rowInRange = ((row + R[i] >= 0) and (row + R[i] < numRows));
        bool colInRange = ((col + C[i] >= 0) and (col + C[i] < numCols));
        bool shown = mineField[row + R[i]][col + C[i]].shown;
        if ((rowInRange) and (colInRange) and (not shown)) {
            move(row + R[i], col + C[i]);
        }
    }
}

/*
 * openSpace
 * arguments: Coordinates of move
 * purpose: Reveal the surrounding spaces of the chosen open space on the board
 * returns: n/a
 */
bool Board::placeFlag(int row, int col)
{
    /* add or remove flag */
    if (mineField[row][col].shown) {
        return false;
    } else if (mineField[row][col].flag) {
        mineField[row][col].flag = false;
        ++flags;
    } else if (flags == 0) {
        return false;
    } else {
        mineField[row][col].flag = true;
        --flags;
    }

    /* flag was successfully placed */
    return true;
}

/*
 * isFlag
 * purpose: Returns whether a flag is at the specified position on the board
 * arguments: The row and col of the cell
 * returns: True if a flag exists, false otherwise
 */
bool Board::isFlag(int row, int col)
{
    return mineField[row][col].flag;
}

/*
 * isShown
 * purpose: Returns whether the cell at the given position is shown
 * arguments: The row and col of the cell
 * returns: True if the flag is shown, false otherwise
 */
bool Board::isShown(int row, int col)
{
    return mineField[row][col].shown;
}

/*
 * numFlags
 * purpose: Returns number of flags available to be placed
 * arguments: n/a
 * returns: Num flags
 */
int Board::numFlags()
{
    return flags;
}

/*
 * getNumRows
 * purpose: Returns number of rows of the board
 * arguments: n/a
 * returns: The number of rows as an integer
 */
int Board::getNumRows()
{
    return numRows;
}

/*
 * getNumCols
 * purpose: Returns number of cols of the board
 * arguments: n/a
 * returns: The number of cols as an integer
 */
int Board::getNumCols()
{
    return numCols;
}

/*
 * getNumber
 * purpose: Returns the number of adjacent bombs to the given cell
 * arguments: The row and col of the cell
 * returns: The number of adjacent bombs
 */
int Board::getNumber(int row, int col) 
{
    /* return the value stored in the given cell if shown and not a bomb */
    if (not mineField[row][col].shown) {
        return 0;
    } else {
        return mineField[row][col].val;
    }
}

/*
 * won
 * arguments: n/a
 * purpose: Determines whether the user has revealed every non-mine square
 * returns: True if every square has been revealed, false otherwise.
 */
bool Board::won()
{
    return spacesLeft <= 0;
}

/*
 * setBombs
 * arguments: The coordinates of the first move
 * purpose: Set the bombs and the rest of the board based on users first move
 * returns: n/a
 */
void Board::setBombs(int row, int col)
{
    //intialize random number generator and its bounds
    random_device rd;
    mt19937 gen(rd());
    int lowerRowBound = 0, upperRowBound = mineField.size() - 1;
    int lowerColBound = 0, upperColBound = mineField[0].size() - 1;

    /* generate random locations of bombs and add them to the bombLocs set */
    while (bombLocs.size() < bombs) {

        //generate random row and col
        uniform_int_distribution<> dis1(lowerRowBound, upperRowBound);
        int ranRow = dis1(gen);
        uniform_int_distribution<> dis2(lowerColBound, upperColBound);
        int ranCol = dis2(gen);

        /* check if random coordinates are  close to first move or duplicates */
        bool close = (abs(ranRow - row) <= 2) and (abs(ranCol - col) <= 2);
        bool dupe = bombLocs.find(make_pair(ranRow, ranCol)) != bombLocs.end();

        /* add bomb and set numbers if not close and not a duplicate */
        if ((not close) and (not dupe)) {
            bombLocs.insert(make_pair(ranRow, ranCol));
            mineField[ranRow][ranCol].val = MINE;
            setNumbers(ranRow, ranCol);            
        }
    }
}

/*
 * setNumbers
 * arguments: Coordinates of a single bomb
 * purpose: Increment the numbers in the surrounding blocks of the new bomb
 * returns: n/a
 */
void Board::setNumbers(int &row, int &col)
{
    /* add one to the adjacent blocks of the cell */
    for (int i = 0; i < ADJACENT_BLOCKS; ++i) {

        /* reveal space if in bounds */
        bool rowInRange = ((row + R[i] >= 0) and (row + R[i] < numRows));
        bool colInRange = ((col + C[i] >= 0) and (col + C[i] < numCols));
        if ((rowInRange) and (colInRange)) {
            ++mineField[row + R[i]][col + C[i]].val;
        }
    }
}

/*
 * getBombPos
 * purpose: Returns the coordinates of a random bomb in the bomb set and then
 *          removes it from the set .
 * arguments: n/a
 * returns: A pair containing the x and y coordinates of the bomb. If set is 
 *          empty it returns a pair with the coordinates -1, -1.
 */
pair<int, int> Board::getBombPos()
{
    /* check if bomb locations is empty */
    if (bombLocs.size() == 0) {
        return OUT_OF_BOMBS;
    }

    /* generate a random bomb index */
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis1(0, bombLocs.size() - 1);

    /* go to the index and get a copy of the pair */
    int randomIndex = dis1(gen);
    pair<int, int> bombPos = *next(bombLocs.begin(), randomIndex);
    
    /* return the location */
    return bombPos;
}


//test fun
void Board::testprint()
{
    for (size_t i = 0; i < mineField.size(); ++i)
    {
        for (size_t j = 0; j < mineField[0].size(); ++j)
        {
            if (mineField[i][j].shown)
                cout << mineField[i][j].val << " ";
            else
                cout << "X" << " ";
        }
        cout << endl;
    }
}