
# Minesweeper
#### Author : Tygan Chin
#### Language : C++
#### Date : November 26, 2023



## Description

A full minesweeper game implementation including:
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


## How to Play

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


## Architecture

- **MainMine.cpp** - Runs the minesweeper game

- **Game_Constants.h** - Defines the window sizes of the minesweeper game and other constants used throughout the program

- **SFMLhelper** - `SFMLhelper.c`, `SFMLhelper.h`
  Defines structs, constants, and functions to help simplify the use of the SFML library

    - **Minesweeper** - `Minesweeper.cpp`, `Minesweeper.h`, `MS_Constants.h`
      Prints out the board and responds to user moves

        - **Board**  - `Board.cpp`, `Board.h`
          Back end representation of the minesweeper game. 2D array of cells that correspond to squares on the board

        - **StartingMenu** - `StartingMenu.cpp`, `StartingMenu.h`, `SM_Constants.h`
          Displays the starting menu and gets the desired difficulty of the user for the game

        - **EndingMenu**  - `EndingMenu.cpp`, `EndingMenu.h`, `EM_Constants.h`
          Displays game stats/information and gets whether the user wants to play the game again

## User Interface


<img width="600" alt="Screenshot 2023-11-26 at 3 21 50 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/61dd9543-21b2-4144-9826-7192850ab361"> 

<img width="300" alt="Screenshot 2023-11-26 at 3 17 26 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/c573ae47-8995-4d4b-a471-433df10e8e7d">
<img width="300" alt="Screenshot 2023-11-26 at 3 17 32 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/70ec0701-307d-4fc4-9a97-c4565a28a12f">
<img width="300" alt="Screenshot 2023-11-26 at 3 18 12 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/6baa1118-d971-41de-8507-3ad8d88c304c">

<img width="300" alt="Screenshot 2023-11-26 at 3 18 20 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/cf0412b9-9ddf-4db6-823b-d925f23a7e06">
<img width="600" alt="Screenshot 2023-11-26 at 3 18 31 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/f5a986db-0369-4654-972f-7197866a4a83">



<img width="300" alt="Screenshot 2023-11-26 at 3 18 15 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/c6a3a1bc-095e-4747-a552-deb602bf7a5c">
<img width="300" alt="Screenshot 2023-11-26 at 3 17 37 PM" src="https://github.com/tyganChin/minesweeper/assets/130794188/55ea053b-5241-402b-8288-1069b2fd1ebd">


