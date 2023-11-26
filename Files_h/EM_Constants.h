/*
 * Project: minesweeper
 * Name: EM_Constants.h
 * Author: Tygan Chin
 * Purpose: Defines the constants used in the implementation of the EndingMenu
 *          class.
 */

#ifndef EM_Constants_H
#define EM_Constants_H

#include "SFMLhelper.h"
#include "Game_Constants.h"
#include <string>

using namespace sf;
using namespace std;

/* window constants */
static const int WIDTH              = MENU_WIDTH;
static const int HEIGHT             = WIDTH / 1.5;
static const float AREA             = WIDTH * HEIGHT;
static const string WINDOW_NAME     = "Ending Screen";
static const Color BACKGROUND_COLOR = WHITE;

/* font */
static const string FONT = "Fonts/Copperplate.ttc";
static const float TEXT_THICKNESS = AREA / 1080000;
static const float DELAY = 0.25;

/* sound constants */
static const int SOUND_EFFECT_DELAY = 3000;
static const soundInfo MUSIC_WIN    = {"Sounds/winMusic.mp3",  75,  1.0,  true};
static const soundInfo MUSIC_LOSS   = {"Sounds/lossMusic.mp3", 75,  1.0,  true};
static const soundInfo EFFECT_WIN   = {"Sounds/winSE.wav",    100, 1.25, false};
static const soundInfo EFFECT_LOSS  = {"Sounds/lossSE.mp3",   100, 1.25, false};

/* background image */
static const string WIN_BACKGROUND_IMAGE  = "Images/winIMG.png";
static const string LOSS_BACKGROUND_IMAGE = "Images/bombIMG.png";
static const float IMAGE_WIDTH = WIDTH / 1.8;
static const float IMAGE_XPOS = (WIDTH - IMAGE_WIDTH) / 2.0f;
static const float IMAGE_YPOS = ((HEIGHT - IMAGE_WIDTH) / 2.0f) - (HEIGHT / 14.25);
static const spriteInfo BACKGROUND_IMAGE = {"FILE", IMAGE_WIDTH, IMAGE_WIDTH, IMAGE_XPOS, IMAGE_YPOS};

/* win total  */
static const string WIN_FILES[] = {"History/winsEasy.txt", "History/winsMedium.txt", "History/winsHard.txt"};
static const Color WIN_TEXT_FILL = GREEN, LOSS_TEXT_FILL = BLACK;
static const rectangleInfo WIN_NUM_BOX = {WIDTH / 12, WIDTH / 12, WIDTH / 14, HEIGHT / 60, TRANSPARENT, 0, BLACK};
static const textInfo WIN_NUM_TEXT = {"Wins: ", FONT, AREA / 13500, TRANSPARENT, AREA / 1080000, BLACK};

/* difficulty */
static const string DIFF_TXT[] = {"E", "M", "H"};
static const Color DIFF_FILL[] = {GREEN, YELLOW, RED};
static const rectangleInfo DIFF_BOX = {WIDTH / 9, HEIGHT / 6, WIDTH / 1.125, 0, TRANSPARENT, 0, BLACK};
static const textInfo DIFF_TEXT = {"INITIAL", FONT, AREA / 6250, TRANSPARENT, TEXT_THICKNESS, BLACK};

/* Win/Loss */
static const string WIN_MESSAGE_TEXT = "YOU WON", LOSS_MESSAGE_TEXT = "YOU LOST";
static const Color WIN_MESSAGE_COLOR = GREEN,     LOSS_MESSAGE_COLOR = RED;
static const rectangleInfo WIN_LOSS_BOX = {WIDTH, HEIGHT, 0, 0, TRANSPARENT, 0, BLACK};
static const textInfo WL_TEXT  = {"MESSAGE",  FONT, AREA / 3272.7, GREEN, TEXT_THICKNESS, BLACK};

/* best time */
static const string TIMES[] = {"History/bestEasy.txt", "History/bestMedium.txt", "History/bestHard.txt"};
static const rectangleInfo BEST_TIME_BOX = {WIDTH / 3.6, HEIGHT / 2.4, WIDTH / 1.8, HEIGHT / 1.935, TRANSPARENT, 0, BLACK};
static const textInfo BEST_TIME_TEXT = {"BEST: ", FONT, AREA / 10800, BLACK, TEXT_THICKNESS, BLACK};

/* curr time */
static const string TIME_LOSS = "...N/A...";
static const rectangleInfo TIME_BOX = {WIDTH / 3.6, HEIGHT / 2.4, WIDTH / 6.25, HEIGHT / 1.935, TRANSPARENT, 0, BLACK};
static const textInfo TIME_TEXT = {"TIME: ", FONT, AREA / 10800, BLACK, TEXT_THICKNESS, BLACK};

/* Quit box */
static const rectangleInfo QUIT_BOX = {WIDTH / 4.5, HEIGHT / 7.5, 0, HEIGHT / 1.154, RED, 0, BLACK};
static const textInfo QUIT_TEXT = {"QUIT", FONT, AREA / 18000, WHITE, TEXT_THICKNESS, BLACK};
static const textBoxInfo QUIT_TEXTBOX = {QUIT_BOX, QUIT_TEXT};

/* Continue box */
static const rectangleInfo CONTINUE_BOX = {WIDTH / 4.5, HEIGHT / 7.5, WIDTH / 1.286, HEIGHT / 1.154, GREEN, 0, BLACK};
static const textInfo CONTINUE_TEXT = {"PLAY AGAIN", FONT, AREA / 18000, WHITE, TEXT_THICKNESS, BLACK};
static const textBoxInfo CONT_TEXTBOX = {CONTINUE_BOX, CONTINUE_TEXT};

#endif
