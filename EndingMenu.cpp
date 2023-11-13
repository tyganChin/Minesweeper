/*
 * Project: minesweeper
 * Name: EndingMenu.cpp
 * Author: Tygan Chin
 * Purpose: Display game info to user and get whether they want to play again
 *          EndingMenu.cpp
 */

#include <string>
#include <iostream>
#include <fstream>
#include "EndingMenu.h"
#include "SFMLhelper.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


/******************************************************\
 *                     CONSTANTS                       *
\******************************************************/

/* window constants */
static const int W_WIDTH            = 900;
static const int W_HEIGHT           = 600;
static const float AREA             = W_WIDTH * W_HEIGHT;
static const string WINDOW_NAME     = "Ending Screen";
static const Color BACKGROUND_COLOR = WHITE;

/* font */
static const string FONT = "Copperplate.ttc";
static const float TEXT_THICKNESS = AREA / 1080000;
static const float DELAY = 0.25;

/* sound constants */
static const int SOUND_EFFECT_DELAY = 3000;
static const soundInfo MUSIC_WIN    = {"MSwin.mp3",    75,  1.0,  true};
static const soundInfo MUSIC_LOSS   = {"MSloss.mp3",   75,  1.0,  true};
static const soundInfo EFFECT_WIN   = {"MSwinSE.wav",  100, 1.25, false};
static const soundInfo EFFECT_LOSS  = {"MSlossSE.mp3", 100, 1.25, false};
static const soundInfo BUTTON       = {"MSbutton.mp3", 100, 1.00, false};

/* background image */
static const string WIN_BACKGROUND_IMAGE  = "MSwinimage.png";
static const string LOSS_BACKGROUND_IMAGE = "MSbombimg.png";
static const float IMAGE_WIDTH = W_WIDTH / 1.8;
static const spriteInfo BACKGROUND_IMAGE = {"FILE", IMAGE_WIDTH, IMAGE_WIDTH, (W_WIDTH - IMAGE_WIDTH) / 2.0f, (W_HEIGHT - IMAGE_WIDTH) / 2.0f};

/* win total  */
static const string WIN_FILES[] = {"winsEasy.txt", "winsMed.txt", "winsHard.txt"};
static const Color WIN_TEXT_FILL = GREEN, LOSS_TEXT_FILL = BLACK;
static const rectangleInfo WIN_NUM_BOX = {W_WIDTH / 12, W_WIDTH / 12, W_WIDTH / 18, W_HEIGHT / 60, TRANSPARENT, 0, BLACK};
static const textInfo WIN_NUM_TEXT = {"W# ", FONT, AREA / 13500, TRANSPARENT, AREA / 1080000, BLACK};

/* difficulty */
static const string DIFF_TXT[] = {"E", "M", "H"};
static const Color DIFF_FILL[] = {GREEN, YELLOW, RED};
static const rectangleInfo DIFF_BOX = {W_WIDTH / 9, W_HEIGHT / 6, W_HEIGHT / (8.0 / 9.0), 0, TRANSPARENT};
static const textInfo DIFF_TEXT = {"INITIAL", FONT, AREA / 6750, TRANSPARENT, TEXT_THICKNESS, BLACK};

/* Win/Loss */
static const string WIN_MESSAGE_TEXT = "YOU WON", LOSS_MESSAGE_TEXT = "YOU LOST";
static const Color WIN_MESSAGE_COLOR = GREEN,     LOSS_MESSAGE_COLOR = RED;
static const rectangleInfo WIN_LOSS_BOX = {W_WIDTH, W_HEIGHT / 1.8, 0, 0, TRANSPARENT, 0, BLACK};
static const textInfo WL_TEXT  = {"MESSAGE",  FONT, AREA / 3272.7, GREEN, TEXT_THICKNESS, BLACK};

/* best time */
static const string TIMES[] = {"bestEasy.txt", "bestMedium.txt", "bestHard.txt"};
static const string TIME_BEST = "BEST: ";
static const rectangleInfo BEST_TIME_BOX = {W_WIDTH / 3.6, W_HEIGHT / 2.4, W_WIDTH / 1.8, W_HEIGHT / 1.935, TRANSPARENT, 0, BLACK};
static const textInfo BEST_TIME_TEXT = {"TEXT", FONT, AREA / 10800, BLACK, TEXT_THICKNESS, BLACK};

/* curr time */
static const string TIME_LOSS = "...N/A...";
static const rectangleInfo TIME_BOX = {W_WIDTH / 3.6, W_HEIGHT / 2.4, W_WIDTH / 6, W_HEIGHT / 1.935, TRANSPARENT, 0, BLACK};
static const textInfo TIME_TEXT = {"TIME: ", FONT, AREA / 10800, BLACK, TEXT_THICKNESS, BLACK};

/* Quit box */
static const rectangleInfo QUIT_BOX = {W_WIDTH / 4.5, W_HEIGHT / 7.5, 0, W_HEIGHT / 1.154, RED, 0, BLACK};
static const textInfo QUIT_TEXT = {"QUIT", FONT, AREA / 18000, WHITE, TEXT_THICKNESS, BLACK};
static const textBoxInfo QUIT_TEXTBOX = {QUIT_BOX, QUIT_TEXT};

/* Continue box */
static const rectangleInfo CONTINUE_BOX = {W_WIDTH / 4.5, W_HEIGHT / 7.5, W_WIDTH / 1.154, W_HEIGHT / 1.154, GREEN, 0, BLACK};
static const textInfo CONTINUE_TEXT = {"PLAY AGAIN", FONT, AREA / 18000, WHITE, TEXT_THICKNESS, BLACK};
static const textBoxInfo CONT_TEXTBOX = {CONTINUE_BOX, CONTINUE_TEXT};

/******************************************************\
 *                    Constructor                      *
\******************************************************/

/* 
 * EndingMenu
 * purpose: Initialize the ending screen window and images
 * arguments: The outcome of the minesweeper game (win or loss), the length of
 *            the game and the diffiuclty it was played on.
 * returns: n/a
 */
EndingMenu::EndingMenu(bool gameWon, Time time, int &difficulty)
{
    /* set window images and sounds */
    setSounds(gameWon);
    setBackgroundImage(gameWon);
    setWinNumber(gameWon, difficulty);
    setDifficulty(difficulty);
    setWinLoss(gameWon);
    setTimes(gameWon, time, difficulty);
    quit = makeTextBox(QUIT_TEXTBOX);
    cont = makeTextBox(CONT_TEXTBOX);
}


/******************************************************\
 *                   Run Function                      *
\******************************************************/

/* 
 * playAgain
 * purpose: Initialize the member variables of the starting menu which includes 
           the render window, audio, and drawings
 * arguments: n/a
 * returns: n/a
 */
bool EndingMenu::playAgain()
{
    /* initialize and center window */
    RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), WINDOW_NAME);
    centerWindow(window, W_WIDTH, W_HEIGHT);

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

            if ((leftClick(event)) and (decision(window))) {
                playSound();
                return again;                 
            }
        }

        /* play music if sound effect finishes */
        if ((soundEffectDone) and (clock.getElapsedTime() >= delay)) {
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
 * arguments: The RenderWindow object
 * returns: True if quit/continue button was pressed, false otherwise
 */
bool EndingMenu::decision(RenderWindow &window)
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
 * arguments: A renderwindow object
 * returns: n/a
 * effects: The ending screen is displayed to user
 */
void EndingMenu::draw(RenderWindow &window)
{
    /* clear the window */
    window.clear(BACKGROUND_COLOR);

    /* draw the images on the window */
    window.draw(backgroundImage);
    drawTextBox(window, difficultyInitial);
    drawTextBox(window, winLoss);
    drawTextBox(window, bestTime);
    drawTextBox(window, time);
    drawTextBox(window, quit);
    drawTextBox(window, cont);

    /* display image */
    window.display();
}

/* 
 * playSound
 * purpose: Play the button sound when quit or continue is pressed
 * arguments: n/a
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
 * arguments: The game outcome
 * returns: n/a
 */
void EndingMenu::setSounds(bool &gameWon)
{
    /* set button sound */
    button = setSound(BUTTON);

    /* set the music and sound effects based on outcome of the game */
    if (gameWon) {
        music = setSound(MUSIC_WIN);
        soundEffect = setSound(EFFECT_WIN);
    } else {
        music = setSound(MUSIC_LOSS);
        soundEffect = setSound(EFFECT_LOSS);
    }
}

/* 
 * setBackgroundImage
 * purpose: Sets the background image of the ending menu.
 * arguments: The game outcome
 * returns: n/a
 */
void EndingMenu::setBackgroundImage(bool &gameWon)
{
    /* create copy of background image struct to allow edit */
    spriteInfo imageInfo = BACKGROUND_IMAGE;

    /* set background image corresponding to win/loss */
    if (gameWon) {
        imageInfo.file = WIN_BACKGROUND_IMAGE;
        backgroundImage = setSprite(imageInfo);
    } else {
        imageInfo.file = LOSS_BACKGROUND_IMAGE;
        backgroundImage = setSprite(imageInfo);
    }
}

/* 
 * setWinNumber
 * purpose: Initializes the number of all time wins of the game
 * arguments: The game outcome and the difficulty 
 * returns: n/a
 */
void EndingMenu::setWinNumber(bool &gameWon, int &difficulty)
{
    /* get the number of wins and update total number if game won */
    string wins = getWins(gameWon, WIN_FILES[difficulty - 1]);

    /* add number of wins to text struct */
    textInfo winNumberText = WIN_NUM_TEXT;
    winNumberText.txt += wins;
    if (gameWon) {
        winNumberText.fill = WIN_TEXT_FILL;
    } else {
        winNumberText.fill = LOSS_TEXT_FILL;
    }

    /* assign win number to member variable */
    winNumber = makeTextBox((textBoxInfo){WIN_NUM_BOX, winNumberText});
}

/* 
 * setDifficulty
 * purpose: Initializes the difficulty initital image of the minesweeper game 
 * arguments: The difficulty of the game
 * returns: n/a
 */
void EndingMenu::setDifficulty(int &difficulty)
{
    /* make info struct */
    textInfo diffTextInfo = DIFF_TEXT;

    /* update initial and color */
    diffTextInfo.txt  = DIFF_TXT[difficulty - 1];
    diffTextInfo.fill = DIFF_FILL[difficulty - 1];

    /* assign difficulty initial to member variable */
    difficultyInitial = makeTextBox((textBoxInfo){DIFF_BOX, diffTextInfo});
}

/* 
 * setWinLoss
 * purpose: Sets the win or loss message in the middle of the screen
 * arguments: The game outcome
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
    winLoss = makeTextBox((textBoxInfo){WIN_LOSS_BOX, winOrLossText});
}

/* 
 * setCurrTime
 * purpose: Sets the time of the minesweeper game played
 * arguments: The game outcome, the time , and the difficulty
 * returns: n/a
 */
void EndingMenu::setTimes(bool &gameWon, Time &gameTime, int &difficulty)
{
    /* set the best time image and return if curr time is the new fastest */
    bool newBest = setBestTime(gameWon, gameTime, TIMES[difficulty - 1]);

    /* set text for current time if game was won*/
    textInfo timeText = TIME_TEXT;
    if (gameWon) {
        timeText.txt += getTime(gameTime);
        if (newBest) {
            timeText.fill = GREEN;
        }
    } else {
        timeText.txt = TIME_LOSS;
    }

    /* assign time to member variable */
    time = makeTextBox((textBoxInfo){TIME_BOX, timeText});
}

/******************************************************\
 *             Image Helper Functions                  *
\******************************************************/


/* 
 * setBestTime
 * purpose: Sets the current best time of the difficulty played by the user
 * arguments: The game outcome, the current time it took to complete the game 
 *            and the difficulty
 * returns: n/a
 */
bool EndingMenu::setBestTime(bool &gameWon, Time &time, string file)
{
    /* read in the best time string and in seconds */
    ifstream fileStream(file);
    string best, seconds;
    fileStream >> best >> seconds;
    fileStream.close();

    /* add best time to text info struct */
    textInfo bestTimeText = BEST_TIME_TEXT;
    bestTimeText.txt += best;

    /* update if curr time is faster than best time */
    bool faster = false;
    if ((gameWon) and (time.asSeconds() <= stoi(seconds))) {

        /* curr time is faster */
        faster = true;
        
        /* update best time file */
        ofstream output(file);
        output << getTime(time) << " " << time.asSeconds();
        output.close();
    }

    /* assign best time to member variable */
    bestTime = makeTextBox((textBoxInfo){BEST_TIME_BOX, bestTimeText});

    return faster;
}

/* 
 * getWins
 * purpose: Get the number of wins listed in the given file
 * arguments: The outcome of the game and the file containing the number 
 *            of wins
 * returns: The number of current wins
 */
string EndingMenu::getWins(bool &gameWon, string filename)
{
    /* open file */
    ifstream file(filename);

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
