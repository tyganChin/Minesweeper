/*****************************************************************************\
 *                                MINESWEEPER                                 *
\*****************************************************************************/
Author : Tygan Chin
Language : C++
Date : November 26, 2023

![Screenshot](https://github.com/tyganChin/minesweeper/blob/main/Screenshot%202023-11-26%20at%203.17.26%20PM.png?raw=true)


/*****************************************************************************\
 *                               Description                                  *
\*****************************************************************************/

A full minesweeper game implementation including : 
    - A starting menu that prompts for the desired difficulty level
    - 3 difficulty levels
        - Easy   :   8x8 with 10 bombs
        - Medium : 16x16 with 40 bombs
        - Hard   : 30x16 with 99 bombs
    - A timer to keep track of the time elapsed during the game
    - The ability to place flags on cells using the space bar
    - Different sound effects and music depending on the state of the game
    - An ending animation where the mines are revealed one by one
    - An ending menu that contains:
        - The time elapsed during the game (if won)
        - The fastest solved game at the certain difficulty
        - The total number of wins at the difficulty
        - The option to play again or quit


/*****************************************************************************\
 *                             How to Play                                    *
\*****************************************************************************/

1. Download the files
2. Ensure you have the Make utility installed in order to run the program
3. Download the SFML library and link to program by including the path to the 
   SFML include and lib folders in the MakeFile IFLAGS and LDFLAGS
4. Type make to build the program
5. Type ./minesweeper in the terminal to run the game
6. Adjust the size of the menus and the board if needed in Game_Constants.h file
    - The rest of the menus and board should scale with these two constants
    - Made to fit a 13.6 inch Macbook
7. Choose a difficulty (easy, medium, or hard)
8. Click on any of the cells (guaranteed to not be a mine)
9. Use the numbers in the cells which represent the number of mines adjacent to 
   it to find out where the bombs are
10. Press the space bar to place a flag on the cell your cursor is hovering 
    over to help keep track of which cells have bombs and which do not
11. Reveal all of the cells without mines in order to window
12. Press anywhere on the window to skip the ending animation
13. Read game stats after the win/loss in the ending menu and click continue to 
    start a new game or quit to close the program


/*****************************************************************************\
 *                           Architecture                                     *
\*****************************************************************************/

+-----------------------------------------------------------------------------+
| - mainMine.cpp - Runs the minesweeper game                                  |
|                                                                             |
| - Game_Constants.h - Defines the window sizes of the minesweeper game and   |
|                       other constants used throughout the program           |
|                                                                             |
| - SFMLhelper - Defines structs, constants, and functions to help simplify   |
|                the use of the SFML library                                  |
|               -> SFMLhelper.c, SFMLhelper.h                                 |
|                                                                             |
|   - Minesweeper - Prints out the board and responds to user moves           |
|   |             -> Minesweeper.cpp, Minesweeper.h, MS_Constants.h           |
|   |                                                                         |
|   | - Board - Back end representation of the minesweeper game. 2D array of  |
|   |           cells that correspond to squares on the board                 |
|   |         -> Board.cpp, Board.h                                           |
|   |                                                                         |
|   | - StartingMenu - Displays the starting menu and gets the desired        |
|   |                  difficulty of the user for the game                    |
|   |                -> StartingMenu.cpp StartingMenu.h, SM_Constants.h       |
|   |                                                                         |
|   | - EndingMenu - Displays game stats/information and gets whether the     |
|   |                the user wants to play the game again                    |
|   |              -> EndingMenu.cpp, EndingMenu.h, EM_Constants.h            |
+-----------------------------------------------------------------------------+