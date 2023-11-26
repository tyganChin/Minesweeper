/*
 * Project: minesweeper
 * Name: MS_Constants.h
 * Author: Tygan Chin
 * Purpose: Defines the constants used in the Minsweeper class
 */

#ifndef MS_Constants_H
#define MS_Constants_H

#include "SFMLhelper.h"
#include "Game_Constants.h"
#include <string>

using namespace sf;
using namespace std;

/* render window */
static const int WIDTH             = GAME_WIDTH;
static const int MED_WIDTH         = WIDTH;
static const int HARD_WIDTH        = WIDTH / 0.53333;
static const int HEIGHT            = WIDTH / 0.88888;
static const int AREA              = WIDTH * HEIGHT;
static const float EASY_SCALE      = 1.0;
static const float MED_SCALE       = 1.0;
static const float HARD_SCALE      = 1.875;
static const float MED_OFFSET      = WIDTH / 2025.0;
static const float HARD_OFFSET     = WIDTH / 592.593;
static const string WINDOW_NAME    = "MINESWEEPER";
static const int FRAMERATE_LIMIT   = 60;

/* end animation */
static const soundInfo EFFECT_WON    = {"Sounds/bombWon.mp3", 50, 1.0, false};
static const soundInfo EFFECT_LOST   = {"Sounds/bombLost.mp3", 50, 1.0, false};
static const string WIN_IMAGE        = "Images/winIMG.png";
static const string LOSS_IMAGE       = "Images/bombIMG.png";
static const int END_ANIMATION_DELAY = 500;
static const int MIN_DELAY           = 100;
static const int DELAY_DECREMENT     = 25;

/* color array */
static const Color COLORS[] = {BLUE, GREEN, RED, MAGENTA, CYAN, YELLOW, BLACK, BLACK};

/* difficulties */
static const int EASY       =  0, MEDIUM       =  1,  HARD       =  2;
static const int EASY_COLS  =  8, MEDIUM_COLS  = 16,  HARD_COLS  = 30;
static const int EASY_ROWS  =  8, MEDIUM_ROWS  = 16,  HARD_ROWS  = 16;
static const int EASY_BOMBS = 10, MEDIUM_BOMBS = 40,  HARD_BOMBS = 99;

/* sounds */
static const soundInfo CLICK_SOUND      = {"Sounds/click.wav", 50, 1.0, false};
static const soundInfo OPEN_SPACE_SOUND = {"Sounds/open.wav", 50, 1.0, false};
static const soundInfo FLAG_SOUND       = {"Sounds/flag.mp3", 50, 1.0, false};

/* font */
static const string FONT = "Fonts/Copperplate.ttc";

/* title */
static const float TITLE_HEIGHT = HEIGHT / 9.5;
static const rectangleInfo TITLE_BOX = {WIDTH, TITLE_HEIGHT, 0, 0, TRANSPARENT, 0, BLACK};
static const textInfo TITLE_TEXT = {"MINESWEEPER", FONT, AREA / 10746.2, BLACK, 0, TRANSPARENT};

/* time */
static const rectangleInfo TIME_BOX = {WIDTH / 5.333, TITLE_HEIGHT, 0, 0, TRANSPARENT, 0, BLACK};
static const textInfo TIME_TEXT = {"Time", FONT, AREA / 18947.3, BLACK, 0, TRANSPARENT};

/* title flag */
static const int MAX_ADJACENT_FLAGS = 8;
static const spriteInfo FLAG_TITLE_IMAGE = {"Images/redFlag.png", HEIGHT / 10.59, HEIGHT / 10.59, WIDTH / 1.17, HEIGHT / 59};
static const rectangleInfo FLAG_COUNT_BOX = {WIDTH / 26.666, HEIGHT / 45,  WIDTH / 1.17, HEIGHT / 26.5, TRANSPARENT, 0, BLACK};
static const textInfo FLAG_COUNT_TEXT = {"Flag Num", FONT, AREA / 28800, WHITE, 0.2, BLACK};

/* grid */
static const float CELL_IMAGE_SCALE = 0.75;
static const Color SHOWN_CELL_COLOR = WHITE;
static const float CELL_THICKNESS_SCALER = 20;
static const float FLAG_XPOS_OFFSET = WIDTH / 28.5714;
static const Color NOT_SHOWN_CELL_COLOR = Color(211, 211, 211);
static const spriteInfo FLAG_CELL_BOX = {"Images/redFlag.png", 0, 0, 0, 0};
static const rectangleInfo CELL_BOX = {0, 0, 0, 0, TRANSPARENT, 0, BLACK};
static const textInfo NUMBER_TEXT = {"NUMBER", FONT, AREA / 14400, TRANSPARENT, 0.2, BLACK};

#endif
