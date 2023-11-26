/*
 * Project: minesweeper
 * Name: Minesweeper.cpp
 * Author: Tygan Chin
 * Purpose: Implementation of the Minesweeper class. Creates and runs the 
 *          minesweeper game.
 */

#include "../Files_h/Minesweeper.h"
#include "../Files_h/StartingMenu.h"
#include "../Files_h/EndingMenu.h"
#include "../Files_h/MS_Constants.h"
#include "../Files_h/Game_Constants.h"
#include "../Files_h/Board.h"
#include "../Files_h/SFMLhelper.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


/******************************************************\
 *                    Constructor                      *
\******************************************************/

/* 
 * Minesweeper
 * purpose: Initialize the board and and images/sounds of the minesweeper game
 * parameters: n/a
 * returns: n/a
 */
Minesweeper::Minesweeper()
{
    /* display starting menu to get difficulty for the game */
    StartingMenu menu;
    difficulty = menu.getDifficulty();

    /* initilialize game with differenet values depending on the difficulty */
    if (difficulty == EASY) {
        gameBoard.setBoard(EASY_COLS, EASY_ROWS, EASY_BOMBS);  
        windowScaler = EASY_SCALE;   
        windowWidth = WIDTH;
        cellScaler = (HEIGHT - TITLE_HEIGHT) / EASY_ROWS;
    } else if (difficulty == MEDIUM) {
        gameBoard.setBoard(MEDIUM_COLS, MEDIUM_ROWS, MEDIUM_BOMBS);
        windowScaler = MED_SCALE;
        windowWidth = MED_WIDTH;
        cellScaler = ((HEIGHT - TITLE_HEIGHT) / MEDIUM_ROWS) - MED_OFFSET; 
    } else if (difficulty == HARD) {
        gameBoard.setBoard(HARD_COLS, HARD_ROWS, HARD_BOMBS);   
        windowScaler = HARD_SCALE;
        windowWidth = HARD_WIDTH;
        cellScaler = ((HEIGHT - TITLE_HEIGHT) / HARD_ROWS) - HARD_OFFSET;     
    } else {
        exit(1);
    }

    /* set the images and sounds */
    setImagesAndAudio();
}


/******************************************************\
 *                   Run Function                      *
\******************************************************/

/* 
 * run
 * purpose: Run the minesweeper game
 * parameters: n/a
 * returns: True if the user wants to play again, false if not
 */
bool Minesweeper::run()
{
    /* create render window */
    RW window(VideoMode(windowWidth, HEIGHT), WINDOW_NAME);
    window.setFramerateLimit(FRAMERATE_LIMIT);

    /* run game */
    Time time = runGame(window);
    
    /* display ending menu and return whether user wants to play again */
    EndingMenu end(gameBoard.won(), time, difficulty);
    return end.playAgain();
}


/******************************************************\
 *               Game Helper Functions                 *
\******************************************************/

/* 
 * runGame
 * purpose: Run the main minesweeper game until user wins/loses or quits
 * parameters: 
 *      RW &window : Render window the game is played on
 * returns: The time elasped during the game
 * note: The program will exit if user exits the window
 */
Time Minesweeper::runGame(RW &window)
{
    /* continue to print board until user wins/loses the game */
    Clock time;
    bool bombHit = false;
    while ((window.isOpen()) and (not bombHit) and (not gameBoard.won()))
    {
        /* draw the board */
        drawGame(window, time.getElapsedTime());

        /* respond to user input */
        Event event;
        while (window.pollEvent(event))
        {
            /* exit window, reveal cell, place flag, or nothing */
            if (event.type == Event::Closed) {
                window.close();   
                exit(1);        
            } else if (SFML.leftClick(event)) {
                bombHit = gameMove(window);
            } else if (SFML.SpacebarPressed(event)) {
                placeFlag(window);
            }
        }
    }

    /* end animation */
    Time currTime = time.getElapsedTime();
    endAnimation(window, currTime);

    /* return elapsed time as a string */
    return currTime;
}

/* 
 * gameMove
 * purpose: Reveal the cell the user clicked on (if valid) and play the 
 *          correspondingsound
 * parameters:
 *      RW &window : Render window the game is played on
 * returns: True if a mine wasn't hit, false otherwise
 */
bool Minesweeper::gameMove(RW &window)
{
    /* get the position of the mouse */
    Vector2i mousePos = Mouse::getPosition(window);

    /* check if the mouse postiion is on the board */
    if (invalidCell(mousePos)) {
        return false ;
    }

    /* make move on board */
    int cell_Row = mousePos.y / cellScaler - (TITLE_HEIGHT / cellScaler);
    int cell_Col = mousePos.x / cellScaler;
    int gameState = gameBoard.move(cell_Row, cell_Col);

    /* play corresponding sound to gameState and return true if mine was hit */
    if (gameState == NUMBERED_SPACE) {
        click_sound.play();
    } else if (gameState == OPEN_SPACE) {
        openSpace_sound.play();
    } else if (gameState == MINE_HIT) {
        return true;
    }

    return false;
}

/* 
 * placeFlag
 * purpose: Place a flag on the cell corresponding to the user's cursor 
 *          (if applicable) and play the corresponding sound.
 * parameters:
 *      RW &window : Render window the game is played on
 * returns: n/a
 * effects: A flag is set to be drawn on the selected cell
 */
void Minesweeper::placeFlag(RW &window)
{
    /* get the current mouse postion */
    Vector2i mousePos = Mouse::getPosition(window);

    /* check if the mouse postiion is on the board */
    if (invalidCell(mousePos)) {
        return;
    }

    /* place flag */
    int cell_Row = (mousePos.y / cellScaler) - (TITLE_HEIGHT / cellScaler);
    int cell_Col = (mousePos.x / cellScaler);
    bool flagPlaced = gameBoard.placeFlag(cell_Row, cell_Col);

    /* play flag noise if flag was placed succssfully */
    if (flagPlaced) {
        flag_sound.play();
    }
}

/* 
 * invalidCell
 * purpose: Determine whether the given mouse position is on a cell
 * parameters:
 *      Vector 2i &mousePos : The postition of the mouse on the render window
 * returns: True if the cursor is not on a valid cell, false otherwise
 */
bool Minesweeper::invalidCell(Vector2i &mousePos)
{
    /* get the height and width of the board displayed on the screen */
    int boardHeight = (cellScaler * gameBoard.getNumRows()) + TITLE_HEIGHT;
    int boardWidth  = (cellScaler * gameBoard.getNumCols());

    /* return whether the mouse position is on the board */
    bool invalidRow = (mousePos.y < TITLE_HEIGHT or mousePos.y > boardHeight);
    bool invalidCol = (mousePos.x < 0 or mousePos.x > boardWidth);
    return (invalidRow or invalidCol);
}

/* 
 * drawGame
 * purpose: Draw the board on the given render window
 * parameters: The game render window and the current elapsed time
 *      RW &window : Render window the game is played on
 *       Time time : The time elapsed during the game
 * returns: n/a
 * Effect: An updated version of the board is drawn to the window
 */
void Minesweeper::drawGame(RW &window, Time currTime)
{
    /* clear the window */
    window.clear(WHITE);

    /* draw the title */
    SFML.drawTextBox(window, title);

    /* draw the time */
    string currTimeString = SFML.getTime(currTime);
    SFML.setString(currTimeString, time.text, time.box);
    SFML.drawTextBox(window, time);

    /* draw flag and flag count */
    string currNumFlags = to_string(gameBoard.numFlags());
    SFML.setString(currNumFlags, flag_amount.text, flag_amount.box);
    window.draw(flag_in_title);
    SFML.drawTextBox(window, flag_amount);

    /* draw grid */
    drawGrid(window);

    /* display the window */
    window.display();
}


/******************************************************\
 *              End Animation Funcs                    *
\******************************************************/

/* 
 * endAnimation
 * purpose: Carries out the end animation of the game, involving either flowers
 *          (if user win) or bombs (if user loses) to be revealed one at a time
 *          and in random order along with their corresponding sounds.
 * parameters: 
 *      RW &window : Render window the game is played on
 *       Time time : Time object containing the time elasped during the game
 * returns: n/a
 * note: The animation can be skipped by clicking anywhere on the screen
 */
void Minesweeper::endAnimation(RW &window, Time time)
{
    /* set animation to either winning or losing */
    int currDelay = END_ANIMATION_DELAY;
    setAnimationImages();

    /* reveal first mine (no delay) */
    bool mineRevealed = reveal(window, time);
    
    /* reveal the mines 1 by 1 after each delay */
    Clock clock;
    while ((window.isOpen()) and (mineRevealed))
    {
        /* start delay */
        Time delay = milliseconds(currDelay);

        /* check if user exited window / exited the animation */
        Event event;
        if (windowExit(window, event)) {
            window.close();
            return;
        }

        /*  Check if the desired delay has passed */
        if ((clock.getElapsedTime() >= delay)) {

            /* reveal mine */
            mineRevealed = reveal(window, time);

            /* update delay and restart clock */
            currDelay = max(MIN_DELAY, currDelay - DELAY_DECREMENT);
            clock.restart();
        }
    }

    sleep(seconds(BUTTON_DELAY));
    window.close();
}

/* 
 * windowExit
 * purpose: Check if the window is clicked by the user which means they would 
 *          like to exit
 * parameters:
 *       RW &window : Render window the game is played on
 *      Event event : The window event
 * returns: True if the user clicked and would like to exit the end animation
 *          false otherwise.
 * note: If the user exists the window itself the program exits.
 */
bool Minesweeper::windowExit(RW &window, Event event)
{
    while (window.pollEvent(event)) {

        /* exit program if window is closed */
        if (event.type == Event::Closed) {
            window.close();
            exit(1);
        }

        /* end animation if screen is clicked */
        if (SFML.leftClick(event)) {
            return true;
        }
    }

    return false;
}

/* 
 * reveal
 * purpose: Reveal another mine and draw the board/play sound
 * parameters: 
 *      RW &window : Render window the game is played on
 *       Time time : The time elapsed during the game
 * returns: True if the mine was revealed, false if there were no mines left 
 *          to reveal.
 */
bool Minesweeper::reveal(RW &window, Time time)
{
    /* return true if all mines have been revealed */
    if (not gameBoard.revealMine()) {
        return false; 
    }
    
    /* draw window with the new mine revealed */
    drawGame(window, time);
    cellReveal.play();
    return true;
}


/******************************************************\
 *                 Set Image Functions                 *
\******************************************************/

/* 
 * setImagesAndAudio
 * purpose: Initialize the images to be drawn on the screen 
 * parameters: n/a
 * returns: n/a
 */
void Minesweeper::setImagesAndAudio()
{
    /* set title */
    rectangleInfo titleBox = TITLE_BOX;
    titleBox.width *= windowScaler;
    title = SFML.makeTextBox(titleBox, TITLE_TEXT);

    /* set time */
    rectangleInfo timeBox = TIME_BOX;
    timeBox.width *= windowScaler;
    timeBox.xPos  *= windowScaler;
    time = SFML.makeTextBox(timeBox, TIME_TEXT);

    /* set flag image and count */
    setFlags();

    /* set the grid images */
    setCellBox();
    setCellText_numbers();
    setCellText_flags();

    /* set the sound effects */
    click_sound     = SFML.setSound(CLICK_SOUND);
    openSpace_sound = SFML.setSound(OPEN_SPACE_SOUND);
    flag_sound      = SFML.setSound(FLAG_SOUND);
}

/* 
 * setFlags
 * purpose: Set the title flag image and flag count
 * parameters: n/a
 * returns: n/a
 */
void Minesweeper::setFlags()
{
    /* set the flag image  */
    spriteInfo titleFlagInfo = FLAG_TITLE_IMAGE;
    titleFlagInfo.xPos *= windowScaler; 
    flag_in_title = SFML.setSprite(titleFlagInfo);

    /* create box for count and set the position based on the screen width */
    rectangleInfo flagTopBox = FLAG_COUNT_BOX;
    flagTopBox.xPos = titleFlagInfo.xPos + FLAG_XPOS_OFFSET;


    /* set flag amoount text */
    flag_amount = SFML.makeTextBox(flagTopBox, FLAG_COUNT_TEXT);
}

/*
 * setCellBox
 * purpose: Set the revealed and hidden cell boxes
 * parameters: n/a
 * returns: n/a
 */
void Minesweeper::setCellBox()
{
    /* set dimensions and outline thickess of the cell */
    int width       = cellScaler;
    int height      = cellScaler;
    float thickness = cellScaler / CELL_THICKNESS_SCALER;

    /* set the rectangle info for the shown and hidden cells */
    rectangleInfo shown  = CELL_BOX;
    rectangleInfo hidden = CELL_BOX;
    shown.width     = hidden.width     = width;
    shown.height    = hidden.height    = height;
    shown.thickness = hidden.thickness = thickness;

    /* update the colors based */
    shown.fill  = SHOWN_CELL_COLOR;
    hidden.fill = NOT_SHOWN_CELL_COLOR;
    
    /* create text boxes for each */
    shown_cell_box     = SFML.createRec(shown);
    not_shown_cell_box = SFML.createRec(hidden);
}

/*
 * setCellText_numbers
 * purpose: Set the numbers of adjecent bombs that go inside of the cells, 
 *          along with their corresponding color;
 * parameters: n/a
 * returns: n/a
 */
void Minesweeper::setCellText_numbers()
{
    for (int i = 0; i < MAX_ADJACENT_FLAGS; ++i) {
        textInfo currNumber = NUMBER_TEXT;
        currNumber.txt = to_string(i + 1);
        currNumber.fill = COLORS[i];
        cellNums[i] = SFML.createText(currNumber);
    }
}

/*
 * setCellText_flags
 * purpose: Set the flag images that go inside of the cells
 * parameters: n/a
 * returns: n/a
 */
void Minesweeper::setCellText_flags()
{
    /* set flag sprite */
    spriteInfo flag = FLAG_CELL_BOX;
    flag.width = cellScaler * CELL_IMAGE_SCALE;
    flag.height = cellScaler * CELL_IMAGE_SCALE;
    cell_flag = SFML.setSprite(flag);
}

/* 
 * setAnimationImages
 * purpose: Set the sounds and images used in the ending animation
 * parameters: n/a
 * returns: n/a
 */
void Minesweeper::setAnimationImages()
{
    /* set cell and cell reveal sound */
    int imgWidth = cellScaler * CELL_IMAGE_SCALE;
    int imgHeight = cellScaler * CELL_IMAGE_SCALE;
    float thickness = cellScaler / CELL_THICKNESS_SCALER;
    if (gameBoard.won()) {

        cellReveal = SFML.setSound(EFFECT_WON);
        end_animation_cell = SFML.createRec(cellScaler, cellScaler, 0, 0, GREEN, thickness, BLACK);
        end_animation_image = SFML.setSprite(WIN_IMAGE, imgWidth, imgHeight, 0, 0);

    } else {

        cellReveal = SFML.setSound(EFFECT_LOST);
        end_animation_cell = SFML.createRec(cellScaler, cellScaler, 0, 0, RED, thickness, BLACK);
        end_animation_image = SFML.setSprite(LOSS_IMAGE, imgWidth, imgHeight, 0, 0);
    }
}


/******************************************************\
 *                Draw Image Functions                 *
\******************************************************/

/* 
 * drawGrid
 * purpose: Draw the grid of cells for the minesweeper game 
 * parameters:
 *      RW &window : Render window the game is played on
 * returns: n/a
 */
void Minesweeper::drawGrid(RW &window)
{
    /* iterate through board drawing each cell */
    for (int row = 0; row < gameBoard.getNumRows(); ++row) {
        for (int col = 0; col < gameBoard.getNumCols(); ++col) {
            drawCell(window, row, col);  
        }
    }
}

/*
 * drawCell
 * purpose: Draw a single cell on the board
 * parameters:
 *      RW &window : Render window the game is played on
 *        int &row : Row of the cell on the board
 *        int &col : Column of the cell on the board
 * returns: n/a
 */
void Minesweeper::drawCell(RW &window, int &row, int &col)
{
    /* calcualte the cell's position */
    int cell_xPos = cellScaler * col;
    int cell_yPos = (cellScaler * row) + TITLE_HEIGHT;

    /* draw the cell */
    if (gameBoard.isShown(row, col)) {

        if (gameBoard.getNumber(row, col) != MINE) {
            /* draw cell box */
            shown_cell_box.setPosition(cell_xPos, cell_yPos);
            window.draw(shown_cell_box);
            
            /* draw the insides of the cell */
            drawOpenCell(window, shown_cell_box, row, col);            
        } else {
            /* draw cell box */
            end_animation_cell.setPosition(cell_xPos, cell_yPos);
            window.draw(end_animation_cell);
            
            /* draw the insides of the cell */
            drawOpenCell(window, end_animation_cell, row, col); 
        }
    } else {

        /* draw cell box */
        not_shown_cell_box.setPosition(cell_xPos, cell_yPos);
        window.draw(not_shown_cell_box);

        /* draw the insides of the cell */
        drawCellFlag(window, not_shown_cell_box, row, col);

    }
}

/*
 * drawOpenCell
 * purpose: Draw a cell that has been revealed
 * parameters: 
 *           RW &window : Render window the game is played on
 *      Rectangle &cell : The drawing of the cell at the row and column
 *             int &row : Row of the cell on the board
 *             int &col : Column of the cell on the board
 * returns: n/a
 */
void Minesweeper::drawOpenCell(RW &window, Rectangle &cell, int &row, int &col)
{
    /* get cell value */
    int cellVal = (gameBoard.getNumber(row, col));

    /* draw the number in the cell or the bomb animation image */
    if (cellVal > 0) {

        Text currNumber = cellNums[cellVal - 1];
        SFML.centerText(currNumber, cell);
        window.draw(currNumber);

    } else if (cellVal == MINE) {

        float offset = ((cellScaler - (cellScaler * CELL_IMAGE_SCALE)) / 2);
        float xPos = cell.getPosition().x + offset;
        float yPos = cell.getPosition().y + offset;
        end_animation_image.setPosition(xPos, yPos);
        window.draw(end_animation_image);

    }
}

/*
 * drawCellFlag
 * purpose: Draw a flag on the hidden cell if necessary
 * parameters: n/a
 *           RW &window : Render window the game is played on
 *      Rectangle &cell : The drawing of the cell at the row and column
 *             int &row : Row of the cell on the board
 *             int &col : Column of the cell on the board
 * returns: n/a
 */
void Minesweeper::drawCellFlag(RW &window, Rectangle &cell, int &row, int &col)
{
    /* return if no flag exists */
    if (not gameBoard.isFlag(row, col)) {
        return;
    }

    /* set the postion of the cell flag image */
    float offset = ((cellScaler - (cellScaler * CELL_IMAGE_SCALE)) / 2);
    float xPos = cell.getPosition().x + offset;
    float yPos = cell.getPosition().y + offset;
    cell_flag.setPosition(xPos, yPos);

    /* draw the cell flag */
    window.draw(cell_flag);
}
