/*
 * Project: minesweeper
 * Name: difficultyMenu.cpp
 * Author: Tygan Chin
 * Purpose: Implementation of the starting menu class for the minesweeper game.
 */

#include <string>
#include <iostream>
#include "StartingMenu.h"
#include "SFMLhelper.h"
#include <SFML/Graphics.hpp>

/******************************************************\
 *                     CONSTANTS                       *
\******************************************************/

/* render window */
static const float WIDTH         = 900;
static const float HEIGHT        = 600;
static const float AREA          = WIDTH * HEIGHT;
static const string WINDOW_NAME  = "MS Menu";
const Color BACKGROUND_COLOR     = BLACK;

/* miscellaneous */
static const int NO_DIFFICULTY_CHOSEN = -1;
static const string FONT = "Copperplate.ttc";
static const float DELAY = 0.25;

/* sounds */
static const soundInfo MUSIC  = {"startingMusic.wav",  100, 1.0, true};
static const soundInfo BUTTON = {"startingButton.mp3", 100, 1.0, false};

/* title */
static const int TITLE_HEIGHT        = HEIGHT / 3;
static const rectangleInfo TITLE_BOX = {WIDTH, TITLE_HEIGHT, 0, 0, WHITE, 0, BLACK};
static const textInfo TITLE_TEXT     = {"MINESWEEPER", FONT, AREA / 7714.2, BLACK,  0, BLACK};
static const textBoxInfo TITLE       = {TITLE_BOX, TITLE_TEXT};

/* difficulty buttons */
static const int NUM_DIFFS        = 3;
static const int DIFF_WIDTH       = WIDTH / NUM_DIFFS;
static const int DIFF_HEIGHT      = HEIGHT - TITLE_HEIGHT;
static const float DIFF_YPOS      = TITLE_HEIGHT;
static const float RECT_THICKNESS = AREA / 108000;
static const Color RECT_OUTLINE   = WHITE;
static const int FONT_SIZE        = AREA / 18000;
static const Color TEXT_COLOR     = BLACK;
static const float TEXT_THICKNESS = 0;
static const Color TEXT_OUTLINE   = BLACK;

/* easy difficulty button */
static const float EASY_XPOS           = (DIFF_WIDTH / NUM_DIFFS) * 0;
static const Color EASY_RECT_COLOR     = Color(0, 200, 0);
static const string EASY_TXT           = "EASY";
static const rectangleInfo EASY_RECT   = {DIFF_WIDTH, DIFF_HEIGHT, EASY_XPOS, DIFF_YPOS, EASY_RECT_COLOR, RECT_THICKNESS, RECT_OUTLINE};
static const textInfo EASY_TEXT        = {EASY_TXT, FONT, FONT_SIZE, TEXT_COLOR, TEXT_THICKNESS, WHITE};
static const textBoxInfo EASY          = {EASY_RECT, EASY_TEXT};

/* medium difficulty button */
static const float MED_XPOS            = (DIFF_WIDTH / NUM_DIFFS) * 1;
static const Color MED_RECT_COLOR      = Color(255, 255, 0);
static const string MED_TXT            = "MEDIUM";
static const rectangleInfo MED_RECT    = {DIFF_WIDTH, DIFF_HEIGHT, MED_XPOS, DIFF_YPOS, MED_RECT_COLOR, RECT_THICKNESS, RECT_OUTLINE};
static const textInfo MED_TEXT         = {MED_TXT, FONT, FONT_SIZE, TEXT_COLOR, TEXT_THICKNESS, WHITE};
static const textBoxInfo MEDIUM        = {MED_RECT, MED_TEXT};

/* hard difficulty button */
static const float HARD_XPOS         = (DIFF_WIDTH / NUM_DIFFS) * 2;
static const Color HARD_RECT_COLOR   = Color(200, 0, 0);
static const string HARD_TXT         = "HARD";
static const rectangleInfo HARD_RECT = {DIFF_WIDTH, DIFF_HEIGHT, HARD_XPOS, DIFF_YPOS, HARD_RECT_COLOR, RECT_THICKNESS, RECT_OUTLINE};
static const textInfo HARD_TEXT      = {HARD_TXT, FONT, FONT_SIZE, TEXT_COLOR, TEXT_THICKNESS, WHITE};
static const textBoxInfo HARD        = {HARD_RECT, HARD_TEXT};

static const textBoxInfo DIFFICULTIES[] = {EASY, MEDIUM, HARD}; 

/******************************************************\
 *                    Constructor                      *
\******************************************************/

/* 
 * StartingMenu
 * purpose: Initialize the member variables of the starting menu which includes 
            the render window, audio, and images.
 * arguments: n/a
 * returns: n/a
 */
StartingMenu::StartingMenu()
{
    /* set background and button sounds */
    music  = setSound(MUSIC);
    button = setSound(BUTTON);

    /* initialze the title */
    menuTitle = makeTextBox(TITLE);

    /* initialize difficulty buttons */
    for (int i = 0; i < NUM_DIFFS; ++i) {
        diffButtons.push_back(makeTextBox(DIFFICULTIES[i]));
    }
}


/******************************************************\
 *                   Run Function                      *
\******************************************************/

/* 
 * getDifficulty
* purpose: Print starting menu to get the desired difficulty of the user
 * arguments: n/a
 * returns: An integer representing the starting difficulty. If no difficulty 
 *          is entered -1 is returned (NO_DIFFICULTY_CHOSEN).
 */
int StartingMenu::getDifficulty()
{
    /* initialize window and center it */
    RenderWindow window(VideoMode(WIDTH, HEIGHT), WINDOW_NAME);
    centerWindow(window, WIDTH, HEIGHT);

    /* draw menu */
    draw(window);

    /* display screen until difficulty is chosen or window is exited */
    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {

            /* check if user exited the window */
            if (event.type == Event::Closed) {
                window.close();     
                return NO_DIFFICULTY_CHOSEN;           
            }

            /* return difficulty if it was pressed */
            if ((leftClick(event)) and (difficultyChosen(window))) {
                playButton();
                return difficulty;                    
            }
        }
    }
    return NO_DIFFICULTY_CHOSEN;
}


/******************************************************\
 *                Helper Functions                     *
\******************************************************/

/* 
 * difficultyChosen
 * purpose: Determines if the user clicked on a difficulty and determines the 
 *          difficulty if so
 * arguments: The starting menu window
 * returns: true if difficulty was picked, false otherwise
 * effects: The difficulty member variable is updated with the corresponding 
 *          integer that represents the difficulty.
 */
bool StartingMenu::difficultyChosen(RenderWindow &window)
{
    /* calcuate mouse position */
    Vector2i mousePos = Mouse::getPosition(window);

    /* check if click was on title box */
    if (mousePos.y < TITLE_HEIGHT) {
        return false;
    }

    /* set difficulty */
    int buttonSize = WIDTH / NUM_DIFFS;
    difficulty = (mousePos.x / buttonSize);

    /* difficulty chosen */
    return true;
}

/* 
 * draw
 * purpose: Draw the starting menu and display it
 * arguments: The staring menu window
 * returns: n/a
 */
void StartingMenu::draw(RenderWindow &window)
{
    /* clear the window */
    window.clear(BACKGROUND_COLOR);

    /* draw the title */
    drawTextBox(window, menuTitle);

    /* draw the difficulty buttons */
    for (int i = 0; i < NUM_DIFFS; ++i) {
        drawTextBox(window, diffButtons[i]);
    }

    /* display */
    window.display();
}

/* 
 * playButton
 * purpose: Play the button sound
 * arguments: n/a
 * returns: n/a
 */
void StartingMenu::playButton()
{
    music.stop();
    button.play();
    sleep(seconds(DELAY));
}