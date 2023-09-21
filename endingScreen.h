//
// Tygan Chin
// endingScree.h
//

#include "SFML/Graphics.hpp"
#include <string>

using namespace sf;
using namespace std;

class endingScreen
{
public:

    endingScreen(bool gameWon, string time, int &difficulty);
    bool playAgain();

private:

    bool again;
    string filename;
    string oldTime;
    void drawSquare(RenderWindow &screen, int height, int width, int x, int y,  
        Color backgroundColor, string output, int fontSize, Color textColor);
    bool decision(Vector2i &mousePos);
    void getOldTime(int &difficulty);
    void bestTime(RenderWindow &screen, string &time, bool &gameWon, int &difficulty);
    bool smaller(string &time, string &oldTime);
};