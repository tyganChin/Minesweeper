//
// Tygan Chin
// mainMine.cpp
// Purpose: main driver of the minesweeper game
//

#include "minesweeper.h"

using namespace std;

int main(void)
{
    bool play = true;
    while (play)
    {
        Minesweeper mineGame;
        play = mineGame.run();        
    }

    return 1;
}