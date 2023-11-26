/*
 * Project: minesweeper
 * Name: StartingMenu.cpp
 * Author: Tygan Chin
 * Purpose: Implementation of the starting menu class for the minesweeper game.
 *          Displays a menu with three difficulty options; easy, medium and
 *          hard. It returns which one the user the user clicks on.
 */

#include <string>
#include <iostream>
#include "../Files_h/StartingMenu.h"
#include "../Files_h/SFMLhelper.h"
#include "../Files_h/SM_Constants.h"
#include "../Files_h/Game_Constants.h"
#include <SFML/Graphics.hpp>


/******************************************************\
 *                    Constructor                      *
\******************************************************/

/* 
 * StartingMenu
 * purpose: Initialize the member variables of the Starting menu
 * parameters: n/a
 * returns: n/a
 */
StartingMenu::StartingMenu()
{
    /* set background and button sounds */
    music  = SFML.setSound(MUSIC);
    button = SFML.setSound(BUTTON);

    /* initialze the title */
    menuTitle = SFML.makeTextBox(TITLE);

    /* initialize difficulty buttons */
    for (int i = 0; i < NUM_DIFFS; ++i) {
        diffButtons.push_back(SFML.makeTextBox(DIFFICULTIES[i]));
    }
}


/******************************************************\
 *                   Run Function                      *
\******************************************************/

/* 
 * getDifficulty
 * purpose: Print starting menu to get the desired difficulty of the user
 * parameters: n/a
 * returns: An integer representing the starting difficulty. If no difficulty 
 *          is entered -1 is returned (NO_DIFFICULTY_CHOSEN).
 */
int StartingMenu::getDifficulty()
{
    /* initialize window and center it */
    RW window(VideoMode(WIDTH, HEIGHT), WINDOW_NAME);
    
    /* play the background music */
    music.play();

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
            if ((SFML.leftClick(event)) and (difficultyChosen(window))) {
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
 * parameters:
 *      RW &window : The render window
 * returns: true if difficulty was picked, false otherwise
 * effects: The difficulty member variable is updated with the corresponding 
 *          integer that represents the difficulty.
 */
bool StartingMenu::difficultyChosen(RW &window)
{
    /* calcuate mouse position */
    Vector2i mousePos = Mouse::getPosition(window);

    /* return if difficulty button wasn't pressed */
    if (mousePos.y < TITLE_HEIGHT) {
        return false;
    }

    /* set difficulty */
    int buttonSize = WIDTH / NUM_DIFFS;
    difficulty = (mousePos.x / buttonSize);

    /* difficulty was chosen */
    return true;
}

/* 
 * draw
 * purpose: Draw the starting menu and display it
 * parameters:
 *      RW &window : The render window
 * returns: n/a
 */
void StartingMenu::draw(RW &window)
{
    /* clear the window */
    window.clear(BACKGROUND_COLOR);

    /* draw the title */
    SFML.drawTextBox(window, menuTitle);

    /* draw the difficulty buttons */
    for (int i = 0; i < NUM_DIFFS; ++i) {
        SFML.drawTextBox(window, diffButtons[i]);
    }

    /* display */
    window.display();
}

/* 
 * playButton
 * purpose: Play the button sound
 * parameters: n/a
 * returns: n/a
 */
void StartingMenu::playButton()
{
    /* stop the music */
    music.stop();

    /* play the button sound */
    button.play();
    sleep(seconds(BUTTON_DELAY));
}
