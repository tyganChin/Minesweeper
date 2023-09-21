//
// Tygan Chin
// Purpose: 
// 
//

#include <vector>
#include <set>
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

class Board
{
public:

    void setBoard(int numRows, int numCols, int numBombs);
    int move(int row, int col);
    void placeFlag(int row, int col);
    bool won();
    void drawBoard(RenderWindow &gameBoard, int &scaler, string t, int &diff, Color bombColor);
    void endAnimation(RenderWindow &screen, bool won, int scaler, string time, int diff);

// private:

    struct Square
    {
        int val;
        bool shown;
        bool flag;
    };

    vector<vector<Square> > mineField;
    set<pair<int, int> > bombLocs;

    int spacesLeft;
    int numRows;
    int numCols;
    size_t bombs;
    int flags;
    Color colors[8];
    bool firstMove;
    pair<int, int> mineHit;

    void openSpace(int &row, int &col);
    void setBombs(int row, int col);
    void setNumbers(int &row, int &col);
    void drawSquare(RenderWindow &gameBoard, int &i, int &j, int &scaler, Color bombColor);
    void setText(RenderWindow &gameBoard, RectangleShape cell, string text, int color, int fontSize);
    void drawFlag(RectangleShape &cell);

    //test funcs
    void testprint();

};