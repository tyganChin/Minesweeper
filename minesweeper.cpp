/*
 * Project: minesweeper
 * Name: 
 * Author: Tygan Chin
 * Purpose: 
 */

#include "minesweeper.h"
#include "StartingMenu.h"
#include "EndingMenu.h"
#include "SFML/Graphics.hpp"
#include "SFMLhelper.h"
#include "board.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


/******************************************************\
 *                     CONSTANTS                       *
\******************************************************/

/* set color array */
static const Color COLORS[] = {BLUE, GREEN, RED, MAGENTA, CYAN, YELLOW, BLACK, BLACK};

/* font */
static const string FONT = "Copperplate.ttc";

/* render window */
static const int WIDTH             = 800;
static const int HEIGHT            = 900;
static const int AREA              = WIDTH * HEIGHT;
static const int HARD_WINDOW_WIDTH = 1500;
static const float EASY_MED_SCALE  = 1.0;
static const float HARD_SCALE      = 1.875;
static const string WINDOW_NAME    = "MINESWEEPER";
static const int FRAMERATE_LIMIT   = 60;

/* end animation */
static const soundInfo EFFECT_WON     = {"MSbombwon.mp3", 50, 1.0, false};
static const string WIN_IMAGE         = "MSwinimage.png";
static const string LOSS_IMAGE        = "MSbombimg.png";
static const int END_ANIMATION_DELAY  = 500;

/* difficulties */
static const int EASY        = 0,  MEDIUM        = 1,   HARD        = 2;
static const int EASY_WIDTH  = 8,  MEDIUM_WIDTH  = 16,  HARD_WIDTH  = 30;
static const int EASY_HEIGHT = 8,  MEDIUM_HEIGHT = 16,  HARD_HEIGHT = 16;
static const int EASY_BOMBS  = 10, MEDIUM_BOMBS  = 40,  HARD_BOMBS  = 99;

/* game status */
static const int MINE_HIT = -1, SPACE_REVEALED = 2, OPEN_SPACE = 1;

/* sounds */
static const soundInfo CLICK_SOUND      = {"MSjumpAudio.wav", 50, 1.0, false};
static const soundInfo OPEN_SPACE_SOUND = {"MSopen.wav", 50, 1.0, false};
static const soundInfo FLAG_SOUND       = {"MSflagDown.mp3", 50, 1.0, false};
static const soundInfo MINE_HIT_SOUND   = {"MSbomblost.mp3", 50, 1.0, false};

/* title */
static const int TITLE_HEIGHT = HEIGHT / 9;
static const rectangleInfo TITLE_BOX = {WIDTH / 2, TITLE_HEIGHT, WIDTH / 4.5, 0, TRANSPARENT, 0, BLACK};
static const textInfo TITLE_TEXT = {"MINESWEEPER", FONT, AREA / 10746.2, BLACK, 0, TRANSPARENT};

/* time */
static const rectangleInfo TIME_BOX = {WIDTH / 5.333, TITLE_HEIGHT, 0, 0, TRANSPARENT, 0, BLACK};
static const textInfo TIME_TEXT = {"Time", FONT, AREA / 18947.3, BLACK, 0, TRANSPARENT};

/* title flag */
static const int MAX_ADJACENT_FLAGS = 8;
static const spriteInfo FLAG_TITLE_IMAGE = {"MSredFlag.png", HEIGHT / 10.59, HEIGHT / 10.59, 25, 15};
static const rectangleInfo FLAG_COUNT_BOX = {WIDTH / 5.333, TITLE_HEIGHT, WIDTH / 1.231, 0, TRANSPARENT, 0, BLACK};
static const textInfo FLAG_COUNT_TEXT = {"Flag Num", FONT, AREA / 28800, WHITE, 0, TRANSPARENT};

/* grid */
static const float CELL_IMAGE_SCALE = 0.75;
static const Color SHOWN_CELL_COLOR = WHITE;
static const Color NOT_SHOWN_CELL_COLOR = Color(211, 211, 211);
static const spriteInfo FLAG_CELL_BOX = {"MSredFlag.png", 0, 0, 0, 0};
static const rectangleInfo CELL_BOX = {0, 0, 0, 0, TRANSPARENT, 5.0, BLACK};
static const textInfo NUMBER_TEXT = {"NUMBER", FONT, AREA / 14400, TRANSPARENT, 0, TRANSPARENT};


/******************************************************\
 *                    Constructor                      *
\******************************************************/

/* 
 * Minesweeper
 * purpose: Initialize the board and and images of the minesweeper game
 * arguments: n/a
 * returns: n/a
 */
Minesweeper::Minesweeper()
{
    /* display starting menu to get difficulty for the game */
    StartingMenu menu;
    difficulty = menu.getDifficulty();

    /* initilialize board with differenet values depending on the difficulty */
    if (difficulty == EASY) {
        gameBoard.setBoard(EASY_WIDTH, EASY_HEIGHT, EASY_BOMBS);        
    } else if (difficulty == MEDIUM) {
        gameBoard.setBoard(MEDIUM_WIDTH, MEDIUM_HEIGHT, MEDIUM_BOMBS);
    } else if (difficulty == HARD) {
        gameBoard.setBoard(HARD_WIDTH, HARD_HEIGHT, HARD_BOMBS);        
    } else {
        exit(1);
    }

    /* set the scaler and width of the render window */
    if ((difficulty == EASY) or (difficulty == MEDIUM)) {
        windowScaler = EASY_MED_SCALE;
        windowWidth = WIDTH;
    } else {
        windowScaler = HARD_SCALE;
        windowWidth = HARD_WINDOW_WIDTH;
    }

    /* set the cell scaler */
    if (difficulty == EASY) {
        cellScaler = (HEIGHT - TITLE_HEIGHT) / EASY_HEIGHT;
    } else {
        cellScaler = (HEIGHT - TITLE_HEIGHT) / MEDIUM_HEIGHT; 
    }

    /* set the window and images */
    setImages();

    /* set the sound effects */
    click_sound = setSound(CLICK_SOUND);
    openSpace_sound = setSound(OPEN_SPACE_SOUND);
    flag_sound = setSound(FLAG_SOUND);
    mineHit_sound = setSound(MINE_HIT_SOUND);
}


/******************************************************\
 *                   Run Function                      *
\******************************************************/

/* 
 * run
 * purpose: Run the minesweeper game
 * arguments: n/a
 * returns: True if the user wants to play again, false if not
 */
bool Minesweeper::run()
{
    /* create render window */
    RenderWindow window(VideoMode(windowWidth, HEIGHT), WINDOW_NAME);
    centerWindow(window, windowWidth, HEIGHT);
    window.setFramerateLimit(FRAMERATE_LIMIT);

    /* run game */
    Time time = runGame(window);

    /* end animation */
    endAnimation(window, time);

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
 * arguments: The RenderWindow the game is played on
 * returns: The timer at the end of the game
 */
Time Minesweeper::runGame(RenderWindow &window)
{
    /* print window and respond to user moves */
    Clock time;
    bool bombHit = false;
    while ((window.isOpen()) and (not bombHit) and (not gameBoard.won()))
    {
        Event event;
        while (window.pollEvent(event))
        {
            /* respond to user move (exit, reveal cell, place flag) */
            if (event.type == Event::Closed) {
                window.close();   
                exit(1);        
            } else if (leftClick(event)) {
                bombHit = gameMove(window);
            } else if (SpacebarPressed(event)) {
                placeFlag(window);
            }
        }
        drawGame(window, time.getElapsedTime());
    }

    /* return elapsed time as a string */
    return time.getElapsedTime();
}

/* 
 * gameMove
 * purpose: Check whether a cell was clicked on and execute move if so 
 * arguments: The render window
 * returns: True if a mine wasn't hit, false otherwise
 */
bool Minesweeper::gameMove(RenderWindow &window)
{
    /* get the position of the mouse */
    Vector2i mousePos = Mouse::getPosition(window);

    /* return if click was not one of the cells */
    if (mousePos.y < TITLE_HEIGHT) {
        return false;
    }

    /* make move on board */
    int cell_Col = mousePos.x / cellScaler;
    int cell_Row = mousePos.y / cellScaler - (TITLE_HEIGHT / cellScaler);
    int gameState = gameBoard.move(cell_Col, cell_Row);

    /* respond to game state */
    if (gameState == SPACE_REVEALED) {
        click_sound.play();
    } else if (gameState == OPEN_SPACE) {
        openSpace_sound.play();
    } else if (gameState == MINE_HIT) {
        mineHit_sound.play();
        return true;
    }

    return false;
}

/* 
 * placeFlag
 * purpose: Place a flag on the board at the desired location 
 * arguments: The render window
 * returns: n/a
 * effects: A flag is set to be drawn on the selected cell
 */
void Minesweeper::placeFlag(RenderWindow &window)
{
    /* get the current mouse postion */
    Vector2i mousePos = Mouse::getPosition(window);

    /* return if the mouse position is not on a cell */
    if (mousePos.y < TITLE_HEIGHT) {
        return;
    }
    
    /* place flag */
    int cell_Col = (mousePos.x / cellScaler);
    int cell_Row = (mousePos.y / cellScaler) - (TITLE_HEIGHT / cellScaler);
    bool flagPlaced = gameBoard.placeFlag(cell_Col, cell_Row);

    /* play flag noise if flag was placed succssfully */
    if (flagPlaced) {
        flag_sound.play();
    }
}

/* 
 * endAnimation
 * purpose: Carries out the end animation of the game, involving either flowers
 *          (if user win) or bombs (if user loses) to be revealed one at a time
 *          and in random order.
 * arguments: The render window and the time elapsed at the end of the game
 * returns: n/a
 */
void Minesweeper::endAnimation(RenderWindow &window, Time time)
{
    //set animation to either winning or losing
    int currDelay = END_ANIMATION_DELAY;
    setAnimationImages();
    
    Clock clock;
    pair<int, int> currBombPos = gameBoard.getBombPos();
    while ((window.isOpen()) and (currBombPos.first != -1))
    {
        /* start delay */
        Time delay = milliseconds(currDelay);

        /* check if user exited window / exited the animation */
        Event event;
        if (windowExit(window, event)) {
            return;
        }

        /*  Check if the desired delay has passed */
        if (clock.getElapsedTime() >= delay) {

            /* reveal square */
            gameBoard.move(currBombPos.first, currBombPos.second);
            cellReveal.play();

            /* get next bomb and increment delay */
            currBombPos = gameBoard.getBombPos();
            currDelay = max(100, currDelay - 25);
            clock.restart();
        }

        /* draw window */
        drawGame(window, time);
    }
}

/* 
 * windowExit
 * purpose: Check if the window is clicked by the user which means they would 
 *          like to exit
 * arguments: The window event
 * returns: True if the user clicked and would like to exit the end animation
 *          false otherwise. If the user exists the window itslef the program
 *          exits.
 */
bool Minesweeper::windowExit(RenderWindow &window, Event event)
{
    while (window.pollEvent(event)) {

        /* exit program if window is closed */
        if (event.type == Event::Closed) {
            window.close();
            exit(1);
        }

        /* end animation if screen is clicked */
        if (leftClick(event)) {
            return true;
        }
    }

    return false;
}

/* 
 * drawGame
 * purpose: Draw the board on the given render window
 * arguments: The game render window and the current elapsed time
 * returns: n/a
 * Effect: An updated version of the board is drawn to the window
 */
void Minesweeper::drawGame(RenderWindow &window, Time currTime)
{
    /* clear the window */
    window.clear(WHITE);

    /* draw the title */
    drawTextBox(window, title);

    /* draw the time */
    string currTimeString = getTime(currTime);
    time.text.setString(currTimeString);
    drawTextBox(window, time);

    /* draw flag and flag count */
    string currNumFlags = to_string(gameBoard.numFlags());
    flag_amount.setString(currNumFlags);
    window.draw(flag_in_title);
    window.draw(flag_amount);

    /* draw grid */
    drawGrid(window);

    /* display the window */
    window.display();
}


/******************************************************\
 *                 Set Image Functions                 *
\******************************************************/

/* 
 * setImages
 * purpose: Initialize the images to be drawn on the screen 
 * arguments: n/a
 * returns: n/a
 */
void Minesweeper::setImages()
{
    /* set title */
    rectangleInfo titleBox = TITLE_BOX;
    titleBox.width *= windowScaler;
    titleBox.xPos *= windowScaler;
    title = makeTextBox((textBoxInfo){titleBox, TITLE_TEXT});

    /* set time */
    rectangleInfo timeBox = TIME_BOX;
    timeBox.width *= windowScaler;
    timeBox.xPos  *= windowScaler;
    time = makeTextBox((textBoxInfo){timeBox, TIME_TEXT});

    /* set flag image and count */
    setFlags();

    /* set the grid images */
    setCellBox();
    setCellText_numbers();
    setCellText_flags();
}

/* 
 * setFlags
 * purpose: Set the title flag image and flag count
 * arguments: n/a
 * returns: n/a
 */
void Minesweeper::setFlags()
{
    /* set the flag image  */
    flag_in_title = setSprite(FLAG_TITLE_IMAGE);
    
    /* create box for count and set the position based on the screen width */
    rectangleInfo flagTopBox = FLAG_COUNT_BOX;
    flagTopBox.xPos *= windowScaler;

    /* set flag amoount text */
    flag_amount = createText(FLAG_COUNT_TEXT);
}

/*
 * setCellBox
 * purpose: Set the revealed and hidden cell boxes
 * arguments: n/a
 * returns: n/a
 */
void Minesweeper::setCellBox()
{
    /* set width and position of cell */
    int width  = cellScaler;
    int height = cellScaler;

    /* set the rectangle info for the shown and hidden cells */
    rectangleInfo shown  = CELL_BOX;
    rectangleInfo hidden = CELL_BOX;

    /* update the colors based */
    shown.fill  = SHOWN_CELL_COLOR;
    hidden.fill = NOT_SHOWN_CELL_COLOR;
    
    /* create text boxes for each */
    shown_cell_box     = createRec(shown);
    not_shown_cell_box = createRec(hidden);
}

/*
 * setCellText_numbers
 * purpose: Set the numbers of adjecent bombs that go inside of the cells
 * arguments: n/a
 * returns: n/a
 */
void Minesweeper::setCellText_numbers()
{
    for (int i = 0; i < MAX_ADJACENT_FLAGS; ++i) {
        textInfo currNumber = NUMBER_TEXT;
        currNumber.txt = to_string(i + 1);
        currNumber.fill = COLORS[i];
        cellNums[i] = createText(currNumber);
    }
}

/*
 * setCellText_flags
 * purpose: Set the flag images that go inside of the cells
 * arguments: 
 * returns: 
 */
void Minesweeper::setCellText_flags()
{
    /* set sprite */
    spriteInfo flag = FLAG_CELL_BOX;
    flag.width = cellScaler * CELL_IMAGE_SCALE;
    flag.height = cellScaler * CELL_IMAGE_SCALE;
    cell_flag = setSprite(flag);
}

/* 
 * setAnimationImages
 * purpose: Set the sounds and images used in the ending animation
 * arguments: n/a
 * returns: n/a
 */
void Minesweeper::setAnimationImages()
{
    /* set cell and cell reveal sound */
    int imgWidth = cellScaler * CELL_IMAGE_SCALE;
    int imgHeight = cellScaler * CELL_IMAGE_SCALE;
    if (gameBoard.won()) {

        cellReveal = setSound(EFFECT_WON);
        end_animation_cell = createRec((rectangleInfo){cellScaler, cellScaler, 0, 0, GREEN});
        end_animation_image = setSprite((spriteInfo){WIN_IMAGE, imgWidth, imgHeight, 0, 0});

    } else {

        cellReveal = mineHit_sound;
        end_animation_cell = createRec((rectangleInfo){cellScaler, cellScaler, 0, 0, RED});
        end_animation_image = setSprite((spriteInfo){LOSS_IMAGE, imgWidth, imgHeight, 0, 0});
    }
}


/******************************************************\
 *                Draw Image Functions                 *
\******************************************************/

/* 
 * drawGrid
 * purpose: Draw the grid of cells for the minesweeper game 
 * arguments: The render window
 * returns: n/a
 */
void Minesweeper::drawGrid(RenderWindow &window)
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
 * arguments: The render window and the row and col of the cell
 * returns: n/a
 */
void Minesweeper::drawCell(RenderWindow &window, int &row, int &col)
{
    /* calcualte the cell's position */
    int cell_xPos = cellScaler * row;
    int cell_yPos = (cellScaler * col) + TITLE_HEIGHT;

    /* draw the cell */
    if (gameBoard.isShown(row, col)) {

        if (gameBoard.getNumber(row, col) != -1) {
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
 * arguments: The render window the cell outline and the row and col of 
 *            the cell
 * returns: n/a
 */
void Minesweeper::drawOpenCell(RenderWindow &window, RectangleShape &cell, int &row, int &col)
{
    /* get cell value */
    int cellVal = (gameBoard.getNumber(row, col));

    /* draw the number in the cell or the bomb animation image */
    if (cellVal > 0) {

        Text currNumber = cellNums[cellVal - 1];
        centerText(currNumber, cell);
        window.draw(currNumber);

    } else if (cellVal == -1) {

        float xPos = cell.getPosition().x + (cellScaler / 6) - 5;
        float yPos = cell.getPosition().y + cellScaler / 6;
        end_animation_image.setPosition(xPos, yPos);
        window.draw(end_animation_image);

    }
}

/*
 * drawCellFlag
 * purpose: Draw a flag on the hidden cell if necessary
 * arguments: n/a
 * returns: n/a
 */
void Minesweeper::drawCellFlag(RenderWindow &window, RectangleShape &cell, int &row, int &col)
{
    /* return if no flag exists */
    if (not gameBoard.isFlag(row, col)) {
        return;
    }

    /* set the postion of the cell flag image */
    float xPos = cell.getPosition().x + (cellScaler / 6) - 5;
    float yPos = cell.getPosition().y + cellScaler / 6;
    cell_flag.setPosition(xPos, yPos);

    /* draw the cell flag */
    window.draw(cell_flag);
}
