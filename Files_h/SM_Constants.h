/*
 * Project: minesweeper
 * Name: SM_Constants.h
 * Author: Tygan Chin
 * Purpose: Defines the constants used in the Starting Menu class.
 */

#ifndef SM_Constants_H
#define SM_Constants_H

#include "SFMLhelper.h"
#include "Game_Constants.h"
#include <string>

using namespace sf;
using namespace std;

/* render window */
static const float WIDTH            = MENU_WIDTH;
static const float HEIGHT           = WIDTH / 1.5;
static const string WINDOW_NAME     = "MS Menu";
static const Color BACKGROUND_COLOR = BLACK;
static const float AREA             = WIDTH * HEIGHT;
  
/* miscellaneous */
static const string FONT = "Fonts/Copperplate.ttc";
static const int NO_DIFFICULTY_CHOSEN = -1;

/* sounds */
static const soundInfo MUSIC  = {"Sounds/startingMusic.wav",  100, 1.0, true};

/* title */
static const float TITLE_HEIGHT      = HEIGHT / 3;
static const rectangleInfo TITLE_BOX = {WIDTH, TITLE_HEIGHT, 0, 0, WHITE, 0, BLACK};
static const textInfo TITLE_TEXT     = {"MINESWEEPER", FONT, AREA / 7714.2, BLACK,  0, BLACK};
static const textBoxInfo TITLE       = {TITLE_BOX, TITLE_TEXT};

/* difficulty buttons */
static const int DIFF_WIDTH       = WIDTH / NUM_DIFFS;
static const int DIFF_HEIGHT      = HEIGHT - TITLE_HEIGHT;
static const float DIFF_YPOS      = TITLE_HEIGHT;
static const float RECT_THICKNESS = AREA / 108000;
static const Color RECT_OUTLINE   = WHITE;
static const float FONT_SIZE      = AREA / 18000;
static const Color TEXT_COLOR     = BLACK;
static const float TEXT_THICKNESS = 0;
static const Color TEXT_OUTLINE   = WHITE;

/* easy difficulty button */
static const float EASY_XPOS         = 0;
static const Color EASY_RECT_COLOR   = Color(0, 200, 0);
static const string EASY_TXT         = "EASY";
static const rectangleInfo EASY_RECT = {DIFF_WIDTH, DIFF_HEIGHT, EASY_XPOS, DIFF_YPOS, EASY_RECT_COLOR, RECT_THICKNESS, RECT_OUTLINE};
static const textInfo EASY_TEXT      = {EASY_TXT, FONT, FONT_SIZE, TEXT_COLOR, TEXT_THICKNESS, TEXT_OUTLINE};
static const textBoxInfo EASY        = {EASY_RECT, EASY_TEXT};

/* medium difficulty button */
static const float MED_XPOS          = DIFF_WIDTH;
static const Color MED_RECT_COLOR    = Color(255, 255, 0);
static const string MED_TXT          = "MEDIUM";
static const rectangleInfo MED_RECT  = {DIFF_WIDTH, DIFF_HEIGHT, MED_XPOS, DIFF_YPOS, MED_RECT_COLOR, RECT_THICKNESS, RECT_OUTLINE};
static const textInfo MED_TEXT       = {MED_TXT, FONT, FONT_SIZE, TEXT_COLOR, TEXT_THICKNESS, TEXT_OUTLINE};
static const textBoxInfo MEDIUM      = {MED_RECT, MED_TEXT};

/* hard difficulty button */
static const float HARD_XPOS         = DIFF_WIDTH * 2;
static const Color HARD_RECT_COLOR   = Color(200, 0, 0);
static const string HARD_TXT         = "HARD";
static const rectangleInfo HARD_RECT = {DIFF_WIDTH, DIFF_HEIGHT, HARD_XPOS, DIFF_YPOS, HARD_RECT_COLOR, RECT_THICKNESS, RECT_OUTLINE};
static const textInfo HARD_TEXT      = {HARD_TXT,  FONT, FONT_SIZE, TEXT_COLOR, TEXT_THICKNESS, TEXT_OUTLINE};
static const textBoxInfo HARD        = {HARD_RECT, HARD_TEXT};

/* array of difficulty text boxes */
static const textBoxInfo DIFFICULTIES[] = {EASY, MEDIUM, HARD}; 

#endif
