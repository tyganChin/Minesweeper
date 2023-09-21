//
// Tygan Chin
// minesweeper.h
// purpose: interface for minesweeper game
//

#include "SFML/Graphics.hpp"
#include "board.h"

using namespace sf;

class Minesweeper
{
public:
    Minesweeper();
    bool run();
    

private:

    Board gameBoard;
    int width;
    int scaler;
    int difficulty;

    string getTime(Time time);

};