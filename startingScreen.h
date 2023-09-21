//
// Tygan Chin
// Purpose: Prints out the starting screen for the user
// startingScreen.h
//

#include "SFML/Graphics.hpp"

using namespace sf;

class startingScreen
{
public:

    startingScreen();
    int getDifficulty();

private:

    int difficulty;
    void drawTitle(RenderWindow &screen);
    void drawDifficulties(RenderWindow &screen);
    bool difficultyChosen(Vector2i &mousePos);
    
};