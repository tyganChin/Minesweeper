/*
 * Project: minesweeper
 * Name: Minesweeper.h
 * Author: Tygan Chin
 * Purpose: Interface for the Minesweeper class. Creates and runs the full
 *          Minesweeper game.
 */

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFMLhelper.h"
#include "Board.h"

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

    /* game board */
    Board gameBoard;
    int difficulty;

    /* window */
    SFMLhelper SFML;
    int windowWidth;
    
    /* scalers */
    float windowScaler;
    float cellScaler;

    /* sounds */
    Sound click_sound;
    Sound openSpace_sound;
    Sound flag_sound;
    Sound cellReveal;

    /* text and rectangles */
    textBox title;
    textBox time;
    textBox flag_amount;
    RectangleShape shown_cell_box;
    RectangleShape not_shown_cell_box;
    RectangleShape end_animation_cell;
    Text cellNums[8];

    /* images */
    Sprite flag_in_title;
    Sprite cell_flag;
    Sprite end_animation_image;

    /* game helper funcs */
    Time runGame(RenderWindow &window);
    bool gameMove(RenderWindow &window);
    void placeFlag(RenderWindow &window);
    bool invalidCell(Vector2i &mousePos);
    void drawGame(RenderWindow &window, Time currTime);

    /* end animation funcs */
    void endAnimation(RenderWindow &window, Time time);
    bool windowExit(RenderWindow &window, Event event);
    bool reveal(RenderWindow &window, Time time);

    /* set image functions */
    void setImagesAndAudio();
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
