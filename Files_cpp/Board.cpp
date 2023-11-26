/*
 * Project: minesweeper
 * Name: Board.cpp
 * Author: Tygan Chin
 * Purpose: Implementation for the minesweeper board class. Allows user to 
 *          create a board of a given width, height, and number of bombs
 *          and be able to interact with the board by choosing squares  
 *          to reveal.
 */

#include "../Files_h/Board.h"
#include "../Files_h/Game_Constants.h"
#include <iostream>
#include <set>
#include <cassert>
#include <random>

/* for adjacent block iteration */
static const int ADJACENT_BLOCKS = 8;
static const int R[] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int C[] = {-1, 0, 1, -1, 1, -1, 0, 1};

/* invalid coordinates */
static const Pos INVALID = make_pair(-1, -1);

/* min number of spaces that must not have bombs in them */
static const int FREE_SPACE = 9;

/******************************************************\
 *                  Constructor                        *
\******************************************************/

/* 
 * Board
 * purpose: Initialize an empty board
 * parameters: n/a
 * returns: n/a
 * note: The set board function must be called in order for the board to be 
 *       fully initialized
 */
Board::Board()
{
    /* set row, col, and bomb dependent vars to 0 */
    numRows = 0;
    numCols = 0;
    bombs = flags = 0;
    spacesLeft = 0;

    /* set member variables */
    random_device rd;
    gen = std::mt19937(rd());
    firstMove = true;
    mineHit = INVALID;
}


/******************************************************\
 *                   Initializer                       *
\******************************************************/

/* 
 * setBoard
 * purpose: Initialize a board of the given dimensions and number of bombs
 * parameters:
 *        int colNum : number of columns in the minesweeper board
 *        int rowNum : number of rows in the minesweeper game
 *      int numBombs : Number of bombs in the board
 * returns: n/a
 * note: This function must be called in order for the class to behave normally
 * expectations: The number of columns, rows and bombs are greater then 0 and 
 *               the number of bombs is less then the total amount of cells 
 *               on the board subtracted by 9 (for first move). CRE if violated
 */
void Board::setBoard(int colNum, int rowNum, int numBombs)
{
    /* validate input */ 
    assert(colNum > 0 and rowNum > 0);
    assert(numBombs > 0 and numBombs < (colNum * rowNum) - FREE_SPACE);

    /* set member variables */
    numRows = rowNum;
    numCols = colNum;
    bombs = flags = numBombs;
    spacesLeft = (numCols * numRows) - numBombs;

    /* initialize empty board */
    Cell EMPTY_CELL = {0, false, false};
    for (int i = 0; i < numRows; ++i) {
        mineField.push_back(vector<Cell>());
        for (int j = 0; j < numCols; ++j) {
            mineField[i].push_back(EMPTY_CELL);
        }
    }
}


/******************************************************\
 *                    Game moves                       *
\******************************************************/

/*
 * move
 * purpose: Executes the user's move
 * parameters:
 *      int row : The row of the cell the user would like to reveal
 *      int col : The column of the cell the user would like to reveal
 * returns: The outcome of the move (numbered space revealed, open space,
 *          no space revealed, or mine hit) defined above.
 * expectations : The coordinates of the given cell are in bounds. CRE if 
 *                violated.
 */ 
int Board::move(int row, int col)
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    /* set bombs and numbers if user first move */
    if (firstMove) {
        setBombs(row, col);    
        firstMove = false;
    }

    /* return if space is already shown */
    if (mineField[row][col].shown) {
        return NO_SPACE; 
    }

    /* check for mine hit and save location of mine if hit*/
    if (mineField[row][col].val == MINE) {
        mineHit = make_pair(row, col);
        return MINE_HIT;
    }

    /* add flag back to total number if necessary */
    if (mineField[row][col].flag) {
        ++flags;        
    }
        
    /* reveal adjacent spaces if open space */
    mineField[row][col].shown = true;
    --spacesLeft;
    if (mineField[row][col].val == 0) {
        openSpace(row, col);        
        return OPEN_SPACE; 
    } else {
        return NUMBERED_SPACE;                
    }
}

/*
 * placeFlag
 * purpose: Place or remove a flag at the given space on the board if possible
 * parameters:
 *      int row : The row of the desired cell
 *      int col : The column of the desired cell
 * returns: True if a flag was placed/removed, false otherwise
 * expectations: The coordinates of the cell are in bounds. CRE if violated.
 */
bool Board::placeFlag(int row, int col)
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    /* add or remove flag if space isn't shown and there are available flags */
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

    /* flag was successfully placed/removed */
    return true;
}


/******************************************************\
 *                      Getters                        *
\******************************************************/

/*
 * won
 * purpose: Determines whether the user has revealed every non-mine cell
 * parameters: n/a
 * returns: True if every cell has been revealed, false otherwise.
 */
bool Board::won()
{
    return spacesLeft <= 0;
}

/*
 * isFlag
 * purpose: Returns whether a flag is at the specified position on the board
 * parameters:
 *      int row : The row of the desired cell
 *      int col : The column of the desired cell
 * returns: True if a flag has been placed on the given cell, false otherwise
 * expectations: The cell coordinates are in bounds. CRE if violated.
 */
bool Board::isFlag(int row, int col)
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    return mineField[row][col].flag;
}

/*
 * isShown
 * purpose: Returns whether the cell at the given position is shown
 * parameters: 
 *      int row : The row of the desired cell
 *      int col : The column of the desired cell
 * returns: True if the cell is shown, false otherwise
 */
bool Board::isShown(int row, int col)
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    return mineField[row][col].shown;
}

/*
 * numFlags
 * purpose: Returns number of flags available to be placed
 * parameters: n/a
 * returns: The number of flags
 */
int Board::numFlags()
{
    return flags;
}

/*
 * getNumRows
 * purpose: Returns number of rows of the board
 * parameters: n/a
 * returns: The number of rows as an integer
 */
int Board::getNumRows()
{
    return numRows;
}

/*
 * getNumCols
 * purpose: Returns number of cols of the board
 * parameters: n/a
 * returns: The number of cols as an integer
 */
int Board::getNumCols()
{
    return numCols;
}

/*
 * getNumber
 * purpose: Returns the number of adjacent bombs of the given cell
 * parameters:
 *      int row : The row of the desired cell
 *      int col : The column of the desired cell
 * returns: The number of adjacent bombs
 * expectations: The given cell coordinates are in bounds. CRE if violated
 */
int Board::getNumber(int row, int col) 
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    /* return the value stored in the given cell if shown and not a bomb */
    if (not mineField[row][col].shown) {
        return 0;
    } else {
        return mineField[row][col].val;
    }
}

/*
 * revealMine
 * purpose: Reveals a random mine on the board or the mine the user hit 
 *          (if not already revealed).
 * parameters: n/a
 * returns: True if a mine was successfully revealed, false if no bombs are 
 *          left.
 */
bool Board::revealMine()
{
    /* check if bomb locations is empty */
    if (bombLocs.size() == 0) {
        return false;
    }

    /* generate random bomb pos or use mine that was hit (if applicable) */
    Pos bombPos;
    if (mineHit == INVALID) {
        uniform_int_distribution<> dis1(0, bombLocs.size() - 1);
        int randomIndex = dis1(gen);
        bombPos = *next(bombLocs.begin(), randomIndex);
    } else {
        bombPos = mineHit;
        mineHit = INVALID;
    }

    /* erase bomb from bomb locations set and reveal it on the board*/
    bombLocs.erase(make_pair(bombPos.first, bombPos.second));
    mineField[bombPos.first][bombPos.second].shown = true;

    return true;
}


/******************************************************\
 *                    Helper Funcs                     *
\******************************************************/

/*
 * openSpace
 * purpose: Reveals the surrounding spaces of the chosen open space on the board
 * parameters:
 *      int row : The row of the desired cell
 *      int col : The column of the desired cell
 * returns: n/a
 * expectations: The given cell coordinates are in bounds. CRE if violated
 */
void Board::openSpace(int &row, int &col)
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    /* reveal the blocks in the direct vicinty of the open space */
    for (int i = 0; i < ADJACENT_BLOCKS; ++i) {

        /* check if in bounds */
        bool rowInRange = ((row + R[i] >= 0) and (row + R[i] < numRows));
        bool colInRange = ((col + C[i] >= 0) and (col + C[i] < numCols));

        if ((rowInRange) and (colInRange)) {
            move(row + R[i], col + C[i]);
        }
    }
}

/*
 * setBombs
 * purpose: Set the bombs and the board values based on user's first move
 * parameters:
 *      int row : The row of the desired cell of the user's first move
 *      int col : The column of the desired cell of the user's first move
 * returns: n/a
 * effects: The bomb locations are set and saved and the adjacent spaces are
 *          incremented to represent the number of bombs it's touching
 * expectations: The given cell coordinates are in bounds. CRE if violated
 */
void Board::setBombs(int row, int col)
{
    /* ensure coordinates are in bounds */
    assert(row >= 0 and row < numRows and col >= 0 and col < numCols);

    /* set distribution bounds for the row and col */
    int lowerRowBound = 0, upperRowBound = mineField.size() - 1;
    int lowerColBound = 0, upperColBound = mineField[0].size() - 1;
    uniform_int_distribution<> dis1(lowerRowBound, upperRowBound);
    uniform_int_distribution<> dis2(lowerColBound, upperColBound);

    /* generate random locations of bombs and add them to the bombLocs set */
    while (bombLocs.size() < bombs) {

        /* generate random row and col */
        int ranRow = dis1(gen);
        int ranCol = dis2(gen);

        /* check if random coordinates are close to first move or duplicates */
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
 * purpose: Increment the numbers in the surrounding blocks of the new bomb.
 *          The numbers reprensent the amount of bombs touching that cell.
 * parameters:
 *      int &bombRow : The row of the new bomb
 *      int &bombCol : The column of the new omb
 * returns: n/a
 * effects: Every valid cell touching the new bomb is incremented by 1.
 */
void Board::setNumbers(int &bombRow, int &bombCol)
{
    /* add number to adjacent space if in bounds and not a mine */
    int row, col;
    for (int i = 0; i < ADJACENT_BLOCKS; ++i) {

        /* get row and column of adjacent block */
        row = bombRow + R[i];
        col = bombCol + C[i];

        /* skip block if coordinates out of range or cell is a bomb */
        bool invalidRow = ((row < 0) or (row >= numRows));
        bool invalidCol = ((col < 0) or (col >= numCols));
        if ((invalidRow) or (invalidCol) or (mineField[row][col].val == -1)) {
            continue;
        }

        /* increment the cell's value (num adjacent bombs) */
        ++mineField[row][col].val; 
    }
}


/******************************************************\
 *                   Test Function                     *
\******************************************************/

/*
 * testprint
 * purpose: Prints out the the board to stdout with X's representing the hidden
 *          spaces
 * parameters: n/a
 * returns: n/a
 */
void Board::testprint()
{
    /* print out the board */
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
    cout << endl;
}
