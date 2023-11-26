/*
 * Project: minesweeper
 * Name: EndingMenu.cpp
 * Author: Tygan Chin
 * Purpose: Implementation for the EndingMenu class. Declares the functions 
 *          that display the ending menu for the user which includes a win/loss
 *          message, the difficulty of the game, the number of wins on the
 *          difficulty, the time elasped, the best time recorded, and quit
 *          and continue buttons.
 */

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include "../Files_h/EndingMenu.h"
#include "../Files_h/SFMLhelper.h"
#include "../Files_h/EM_Constants.h"
#include "../Files_h/Game_Constants.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


/******************************************************\
 *                    Constructor                      *
\******************************************************/

/* 
 * EndingMenu
 * purpose: Initialize the ending screen window and images
 * parameters:
 *         bool gameWon : Whether or not the game was won
 *            Time time : The time elasped during the game   
 *      int &difficulty : The difficulty the game was played on
 * returns: n/a
 * expectations: The difficulty is between 0 and the total number of 
 *               difficulties
 */
EndingMenu::EndingMenu(bool gameWon, Time time, int &difficulty)
{
    /* check if difficulty is valid */
    assert(difficulty >= 0 and difficulty < NUM_DIFFS);

    /* set window images and sounds */
    setSounds(gameWon);
    setBackgroundImage(gameWon);
    setWinNumber(gameWon, difficulty);
    setDifficulty(difficulty);
    setWinLoss(gameWon);
    setTimes(gameWon, time, difficulty);
    quit = SFML.makeTextBox(QUIT_TEXTBOX.rectangleData, QUIT_TEXTBOX.textData);
    cont = SFML.makeTextBox(CONT_TEXTBOX.rectangleData, CONT_TEXTBOX.textData);
}


/******************************************************\
 *                   Run Function                      *
\******************************************************/

/* 
 * playAgain
 * purpose: Display the ending menu to user and get user's decision to play
 *          game again or not.
 * parameters: n/a
 * returns: n/a
 */
bool EndingMenu::playAgain()
{
    /* initialize and center window */
    RW window(VideoMode(WIDTH, HEIGHT), WINDOW_NAME);

    /* delay for sound effect */
    Clock clock;
    Time delay = milliseconds(SOUND_EFFECT_DELAY);
    soundEffect.play();

    /* draw ending screen */
    draw(window);

    /* display ending screen until quit or continue buttons are pressed */
    bool soundEffectDone = false;
    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {

            /* check if user exited the window */
            if (event.type == Event::Closed) {
                window.close();    
                return false;            
            }

            if ((SFML.leftClick(event)) and (decision(window))) {
                playSound();
                return again;                 
            }
        }

        /* play music if sound effect finishes */
        if ((not soundEffectDone) and (clock.getElapsedTime() >= delay)) {
            soundEffect.stop();
            soundEffectDone = true;
            music.play();
        }
    }
    return false;
}


/******************************************************\
 *            Window Helper Functions                  *
\******************************************************/

/* 
 * decision
 * purpose: Determine whether quit/continue button was pressed and save result 
 *          if so.
 * parameters: 
 *      RW &window : The render window
 * returns: True if quit/continue button was pressed, false otherwise
 * effects: The again bool member variable is updated if quit/continue is
 *          pressed.
 */
bool EndingMenu::decision(RW &window)
{
    /* calculate position of mouse */
    Vector2i mousePos = Mouse::getPosition(window);

    /* check if click was not on the continue or quit buttons */
    bool tooHigh = (mousePos.y <= quit.box.getPosition().y);
    bool quitButton = (mousePos.x <= quit.box.getSize().x);
    bool continueButton = (mousePos.x >=  cont.box.getPosition().x);
    if (((not quitButton) and (not continueButton)) or (tooHigh)) {
        return false;        
    }

    /* set play again to true if play again was pressed or false if quit */
    if (quitButton) {
        again = false;        
    } else {
        again = true;
    }

    return true;
}

/* 
 * draw
 * purpose: Draw the ending screen images to the render window
 * parameters:
 *      RW &window : The render window
 * returns: n/a
 * effects: The ending screen is displayed to user
 */
void EndingMenu::draw(RW &window)
{
    /* clear the window */
    window.clear(BACKGROUND_COLOR);

    /* draw the images on the window */
    window.draw(backgroundImage);
    SFML.drawTextBox(window, winNumber);
    SFML.drawTextBox(window, difficultyInitial);
    SFML.drawTextBox(window, winLoss);
    SFML.drawTextBox(window, bestTime);
    SFML.drawTextBox(window, time);
    SFML.drawTextBox(window, quit);
    SFML.drawTextBox(window, cont);

    /* display image */
    window.display();
}

/* 
 * playSound
 * purpose: Play the button sound when quit or continue is pressed
 * parameters: n/a
 * returns: n/a
 */
void EndingMenu::playSound()
{
    /* stop the music and sound effect */
    music.stop();
    soundEffect.stop();

    /* play the button */
    button.play();
    sleep(seconds(DELAY));
}


/******************************************************\
 *                 Image Functions                     *
\******************************************************/

/* 
 * setSounds
 * purpose: Set the music, button sound and sound effect for the menu.
 * parameters:
 *      bool &gameWon : The outcome of the game
 * returns: n/a
 */
void EndingMenu::setSounds(bool &gameWon)
{
    /* set button sound */
    button = SFML.setSound(BUTTON);

    /* set the music and sound effects based on outcome of the game */
    if (gameWon) {
        music = SFML.setSound(MUSIC_WIN);
        soundEffect = SFML.setSound(EFFECT_WIN);
    } else {
        music = SFML.setSound(MUSIC_LOSS);
        soundEffect = SFML.setSound(EFFECT_LOSS);
    }
}

/* 
 * setBackgroundImage
 * purpose: Sets the background image of the ending menu.
 * parameters:
 *      bool &gameWon : The outcome of the game
 * returns: n/a
 */
void EndingMenu::setBackgroundImage(bool &gameWon)
{
    /* create copy of background image struct to allow edit */
    spriteInfo imageInfo = BACKGROUND_IMAGE;

    /* set background image corresponding to win/loss */
    if (gameWon) {
        imageInfo.file = WIN_BACKGROUND_IMAGE;
        backgroundImage = SFML.setSprite(imageInfo);
    } else {
        imageInfo.file = LOSS_BACKGROUND_IMAGE;
        backgroundImage = SFML.setSprite(imageInfo);
    }
}

/* 
 * setWinNumber
 * purpose: Initializes the number of all time wins of the game
 * parameters:
 *        bool &gameWon : The outcome of the game
 *      int &difficulty : The difficulty of the game played
 * returns: n/a
 */
void EndingMenu::setWinNumber(bool &gameWon, int &difficulty)
{
    /* get the number of wins and update total number if game won */
    string wins = getWins(gameWon, WIN_FILES[difficulty]);
  
    /* add number of wins to text struct */
    textInfo winNumberText = WIN_NUM_TEXT;
    winNumberText.txt += wins;
    if (gameWon) {
        winNumberText.fill = WIN_TEXT_FILL;
    } else {
        winNumberText.fill = LOSS_TEXT_FILL;
    }

    /* assign win number to member variable */
    winNumber = SFML.makeTextBox(WIN_NUM_BOX, winNumberText);
}

/* 
 * setDifficulty
 * purpose: Initializes the difficulty initital image of the minesweeper game 
 * parameters:
 *      int &difficulty : The difficulty of the game played
 * returns: n/a
 */
void EndingMenu::setDifficulty(int &difficulty)
{
    /* make info struct */
    textInfo diffTextInfo = DIFF_TEXT;

    /* update initial and color */
    diffTextInfo.txt  = DIFF_TXT[difficulty];
    diffTextInfo.fill = DIFF_FILL[difficulty];

    /* assign difficulty initial to member variable */
    difficultyInitial = SFML.makeTextBox(DIFF_BOX, diffTextInfo);
}

/* 
 * setWinLoss
 * purpose: Sets the win or loss message in the middle of the screen
 * parameters:
 *      bool &gameWon : The outcome of the game
 * returns: name
 */
void EndingMenu::setWinLoss(bool &gameWon)
{
    /* initialize text info struxt */
    textInfo winOrLossText = WL_TEXT;

    /* update message and color */
    if (gameWon) {
        winOrLossText.txt = WIN_MESSAGE_TEXT;
        winOrLossText.fill = WIN_MESSAGE_COLOR;
    } else {
        winOrLossText.txt = LOSS_MESSAGE_TEXT;
        winOrLossText.fill = LOSS_MESSAGE_COLOR;
    }

    /* assign win/loss message to member variable */
    winLoss = SFML.makeTextBox(WIN_LOSS_BOX, winOrLossText);
}

/* 
 * setCurrTime
 * purpose: Sets the time of the minesweeper game played
 * parameters:
 *        bool &gameWon : The outcome of the game
 *       Time &gameTime : The time of the elapsed game
 *      int &difficulty : The difficulty of the game played
 * returns: n/a
 */
void EndingMenu::setTimes(bool &gameWon, Time &gameTime, int &difficulty)
{
    /* set the best time image and return if curr time is the new fastest */
    bool newBest = setBestTime(gameWon, gameTime, TIMES[difficulty]);

    /* set text for current time if game was won*/
    textInfo timeText = TIME_TEXT;
    if (gameWon) {
        timeText.txt += SFML.getTime(gameTime);
        if (newBest) {
            timeText.fill = GREEN;
        }
    } else {
        timeText.txt = TIME_LOSS;
    }

    /* assign time to member variable */
    time = SFML.makeTextBox(TIME_BOX, timeText);
}

/* 
 * setBestTime
 * purpose: Sets the current best time of the difficulty played by the user
 * parameters: The game outcome, the current time it took to complete the game 
 *            and the difficulty
 *       bool &gameWon : The outcome of the game
 *      Time &gameTime : The time of the elapsed game
 *         int &string : The file name storing the fastest time of the current
 *                        difficulty 
 * returns: True if the given time is faster and the game was won, false
 *          otherwise.
 * Effects: The best time file is updated if the time was faster.
 * Expectations: The given file is valid. CRE if violated
 */
bool EndingMenu::setBestTime(bool &gameWon, Time &time, string file)
{
    /* read in file */
    ifstream fileStream(file);
    assert(fileStream.is_open());

    /* read in the best time string and in seconds */
    string best, seconds;
    fileStream >> best >> seconds;
    fileStream.close();

    /* add best time to text info struct */
    textInfo bestTimeText = BEST_TIME_TEXT;
    bestTimeText.txt += best;

    /* assign best time to member variable */
    bestTime = SFML.makeTextBox(BEST_TIME_BOX, bestTimeText);

    /* update if curr time is faster than best time */
    if ((gameWon) and (time.asSeconds() <= stoi(seconds))) {
        ofstream output(file);
        assert(output.is_open());
        output << SFML.getTime(time) << " " << time.asSeconds();
        output.close();
        return true;
    }
    return false;
}


/******************************************************\
 *             Image Helper Function                   *
\******************************************************/

/* 
 * getWins
 * purpose: Get the number of wins listed in the given file
 * parameters: 
 *       bool &gameWon : The outcome of the game
 *         int &string : The file name storing the number of games won on the 
 *                       current difficulty
 * returns: The number of current wins
 * expectations: The given file is valid. CRE if violated.
 */
string EndingMenu::getWins(bool &gameWon, string filename)
{
    /* open file */
    ifstream file(filename);
    assert(file.is_open());

    /* read in number of wins */
    string wins;
    file >> wins;
    file.close();

    /* add one to the win total if user won and update file */
    if (gameWon) {

        /* incremenet string */
        wins = to_string(stoi(wins) + 1);

        /* save updated win count to file */
        ofstream output(filename);
        output << wins;
        output.close();
    }

    /* return num wins */
    return wins;
}
