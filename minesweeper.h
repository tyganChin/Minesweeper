/*
 * Project: minesweeper
 * Name: 
 * Author: Tygan Chin
 * Purpose: 
 */

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFMLhelper.h"
#include "board.h"

using namespace std;
using namespace sf;

class Minesweeper
{
public:

    /* constructor */
    Minesweeper();

    /* run function */
    bool run();
    
private:

    /* member variables */
    Board gameBoard;
    int windowWidth;
    int windowScaler;
    int cellScaler;
    int difficulty;
    Sound click_sound, openSpace_sound, flag_sound, mineHit_sound;
    Sound cellReveal;

    /* images */
    textBox title;
    textBox time;
    RectangleShape shown_cell_box;
    RectangleShape not_shown_cell_box;
    RectangleShape end_animation_cell;
    Text flag_amount;
    Text cellNums[8];
    Sprite flag_in_title;
    Sprite cell_flag;
    Sprite end_animation_image;

    /* game helper funcs */
    Time runGame(RenderWindow &window);
    bool gameMove(RenderWindow &window);
    void placeFlag(RenderWindow &window);
    void endAnimation(RenderWindow &window, Time time);
    bool windowExit(RenderWindow &window, Event event);
    void drawGame(RenderWindow &window, Time currTime);

    /* set image functions */
    void setImages();
    void setWindowDimensions();
    void setFlags();
    void setCellBox();
    void setCellText_numbers();
    void setCellText_flags();
    void setAnimationImages();

    /* draw image funtions */
    void drawGrid(RenderWindow &window);
    void drawCell(RenderWindow &window, int &row, int &col);
    void drawOpenCell(RenderWindow &window, RectangleShape &cell, int &row, int &col);
    void drawCellFlag(RenderWindow &window, RectangleShape &cell, int &row, int &col);
};
