//
// Tygan Chin
// Purpose: 
// 
//

#include "board.h"
#include <random>
#include <set>
#include <iostream>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <random>

using namespace std;
using namespace sf;

// Board
// arguments: Number of rows, columns, and bombs
// purpose: Initialize a board of the given size
// returns: n/a
void Board::setBoard(int rowNum, int colNum, int numBombs)
{
    //set memeber variables
    numRows = rowNum, numCols = colNum, bombs = flags = numBombs;
    spacesLeft = (numCols * numRows) - numBombs;
    firstMove = true;

    //initialize board
    for (int i = 0; i < numRows; ++i)
    {
        mineField.push_back(vector<Square>());
        for (int j = 0; j < numCols; ++j)
        {
            Square newSquare;
            newSquare.val = 0, newSquare.shown = false, newSquare.flag = false;
            mineField[i].push_back(newSquare);
        }
    }

    //set color array
    colors[0] = Color::Blue;
    colors[1] = Color::Green;
    colors[2] = Color::Red;
    colors[3] = Color::Magenta;
    colors[4] = Color::Cyan;
    colors[5] = Color::Yellow;
    colors[6] = Color::Black;
    colors[7] = Color::Black;
}

// move
// arguments: Coordinates of user move 
// purpose: Executes the users move
// returns: False if a mine was hit, true otherwise
int Board::move(int row, int col)
{
    //set bombs and numbers if user first move
    if (firstMove)
    {
        setBombs(row, col);    
        firstMove = false;
    }

    //add flag back to total number if necessary
    if (mineField[row][col].flag)
        ++flags;

    //respond to users move
    if (mineField[row][col].shown)
        return 0;
    else if (mineField[row][col].val == 0)
    {
        openSpace(row, col); 
        return 1;
    } 
    else if (mineField[row][col].val == -1)
    {
        mineHit = make_pair(row, col);
        return -1;
    }
    else
    {        
        mineField[row][col].shown = true;
        --spacesLeft;
        return 2;
    }
}

// openSpace
// arguments: Coordinates of move
// purpose: Reveal the surrounding spaces of the chosen open space on the board
// returns: n/a
void Board::placeFlag(int row, int col)
{
    if (mineField[row][col].shown)
        return;
    else if (mineField[row][col].flag)
    {
        mineField[row][col].flag = false;
        ++flags;
    }
    else if (flags > 0)
    {
        mineField[row][col].flag = true;
        --flags;
    }
}

// openSpace
// arguments: Coordinates of move
// purpose: Reveal the surrounding spaces of the chosen open space on the board
// returns: n/a
void Board::openSpace(int &row, int &col)
{
    //reveal space and decrement space count
    mineField[row][col].shown = true;
    --spacesLeft;

    //reveal the blocks in the direct vicinty of the open space
    for (int i = row - 1; i <= row + 1; ++i)
    {
        if ((i < 0) or (i >= numRows))
            continue;

        for (int j = col - 1; j <= col + 1; ++j)
        {
            bool OutOfRange = (j < 0) || (j >= numCols);
            bool CurrCell = (i == row) && (j == col);
            bool AlreadyShown = mineField[i][j].shown;

            if ((not OutOfRange) and (not CurrCell) and (not AlreadyShown)) 
                move(i, j);                   
        }

    }
}

// won
// arguments: n/a
// purpose: Determines whether the user has revealed every non-mine square
// returns: True if every square has been revealed, false otherwise.
bool Board::won()
{
    return spacesLeft <= 0;
}

// setBombs
// arguments: The coordinates of the first move
// purpose: Set the bombs and the rest of the board based on users first move
// returns: n/a
void Board::setBombs(int row, int col)
{
    //intialize random number generator and its bounds
    random_device rd;
    mt19937 gen(rd());
    int lowerRowBound = 0, upperRowBound = mineField.size() - 1;
    int lowerColBound = 0, upperColBound = mineField[0].size() - 1;

    //generate random locations of bombs
    while (bombLocs.size() < bombs)
    {
        //generate random row and col
        uniform_int_distribution<> dis1(lowerRowBound, upperRowBound);
        int ranRow = dis1(gen);
        uniform_int_distribution<> dis2(lowerColBound, upperColBound);
        int ranCol = dis2(gen);

        //check to see if coordinates are at or surrounding the first move
        bool isClose = ((abs(ranRow - row) <= 2) and (abs(ranCol - col) <= 2));
        bool isDuplicate = (bombLocs.find(make_pair(ranRow, ranCol)) 
                            != bombLocs.end());

        if ((not isClose) and (not isDuplicate))
        {
            bombLocs.insert(make_pair(ranRow, ranCol));
            mineField[ranRow][ranCol].val = -1;
            setNumbers(ranRow, ranCol);            
        }
    }
}

// setNumbers
// arguments: Coordinates of a single bomb
// purpose: Increment the numbers in the surrounding blocks of the new bomb
// returns: n/a
void Board::setNumbers(int &row, int &col)
{
    //update the blocks that surround the mine
    for (int i = row - 1; i <= row + 1; ++i)
    {
        if ((i < 0) or (i >= numRows))
            continue;

        for (int j = col - 1; j <= col + 1; ++j)
            if ((j >= 0) and (j < numCols) and (mineField[i][j].val != -1)) 
                ++mineField[i][j].val;
    }
}

// drawBoard 
// purpose: Draws the minesweeper board 
void Board::drawBoard(RenderWindow &gameBoard, int &scaler, string t, int &diff, Color bombColor)
{
    //set title scaler
    float titleScale = 1.0;
    if (diff == 3)
        titleScale = 1.875;
    
    //draw the title
    RectangleShape title(Vector2f(400 * titleScale, 100));
    title.setPosition(200 * titleScale, 0);
    setText(gameBoard, title, "MINESWEEPER", 7, 67);

    //draw time
    RectangleShape timer(Vector2f(150 * titleScale, 100));
    timer.setPosition(0, 0);
    setText(gameBoard, timer, t, 7, 38);

    //draw number of flags
    RectangleShape flagNum(Vector2f(150 * titleScale, 150 * titleScale));
    flagNum.setPosition(650 * titleScale, 0);
    Texture texture;
    texture.loadFromFile("/Users/tyganchin/Downloads/MSredFlag.png");
    Sprite flag;
    flag.setTexture(texture);
    flag.setPosition(flagNum.getPosition().x + 25, flagNum.getPosition().y + 15);
    Vector2f newSize(85, 85);
    flag.setScale(newSize.x / flag.getLocalBounds().width,
                newSize.y / flag.getLocalBounds().height);
    gameBoard.draw(flag);
    
    //create title
    Font font;
    font.loadFromFile(("/System/Library/Fonts/Supplemental/Copperplate.ttc"));
    Text output(to_string(flags), font, 25);
    output.setFillColor(Color::White);

    //center title to middle of square
    FloatRect bounds = output.getLocalBounds();
    if ((diff == 1) or (diff == 2))
    {
        output.setOrigin(bounds.left + bounds.width / 2.0f, 
                bounds.top + bounds.height / 2.0f);
        output.setPosition(flagNum.getPosition().x + flagNum.getSize().x / 
                2.15f, flagNum.getPosition().y + flagNum.getSize().y / 3.5f);
    }
    else
    {
        output.setOrigin(bounds.left + bounds.width / 2.0f, 
                bounds.top + bounds.height / 2.0f);
        output.setPosition(650 * titleScale + 65, 42);
    }
    gameBoard.draw(output);



    //iterate through board drawing each square (pixel to coordinate convertion)
    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j)
            drawSquare(gameBoard, i, j, scaler, bombColor);             
}

// drawSquare
// purpose: draws a square using the given inforamtion
void Board::drawSquare(RenderWindow &gameBoard, int &i, int &j, int &scaler, Color bombColor)
{
    //create square
    RectangleShape cell(Vector2f(scaler, scaler));
    cell.setPosition(j * scaler, (i * scaler) + 100);
    cell.setOutlineThickness(5.0f); // Border thickness
    cell.setOutlineColor(Color::Black); // Border color

    //set insides
    int squareVal = mineField[i][j].val;
    if (not mineField[i][j].shown)
        cell.setFillColor(Color(211, 211, 211));
    else if (squareVal == -1)
    {
        cell.setFillColor(bombColor);    
        Texture texture;
        if (bombColor == Color::Red) 
            texture.loadFromFile("/Users/tyganchin/Downloads/MSbombimg.png");
        else
            texture.loadFromFile("/Users/tyganchin/Downloads/MSwinimage.png");

        Sprite endPicture;
        endPicture.setTexture(texture);
        endPicture.setPosition(cell.getPosition().x + (scaler / 6) - 5, cell.getPosition().y + (scaler / 6) - 5);
        Vector2f newSize(scaler * 0.75, scaler * 0.75);
        endPicture.setScale(newSize.x / endPicture.getLocalBounds().width,
                    newSize.y / endPicture.getLocalBounds().height);
        gameBoard.draw(cell);
        gameBoard.draw(endPicture);

    }
    else
        cell.setFillColor(Color::White);

    //draw end picutre if necessary
    if ((squareVal != -1) or (not mineField[i][j].shown))
        gameBoard.draw(cell);

    //draw flag if necessary
    if ((not mineField[i][j].shown) and (mineField[i][j].flag))
    {
        //load in texture
        Texture texture;
        texture.loadFromFile("/Users/tyganchin/Downloads/MSredFlag.png");

        //create sprite
        Sprite flag;
        flag.setTexture(texture);
        flag.setPosition(cell.getPosition().x + (scaler / 6) - 5, cell.getPosition().y + scaler / 6);

        //scale sprite to fit in the cell
        Vector2f newSize(scaler * 0.75, scaler * 0.75);
        flag.setScale(newSize.x / flag.getLocalBounds().width,
                    newSize.y / flag.getLocalBounds().height);

        //draw
        gameBoard.draw(flag);
    }

    //number if cell is close to mine
    if ((squareVal > 0) and (mineField[i][j].shown))
        setText(gameBoard, cell, to_string(squareVal), squareVal - 1, 50);

}

// setText
// sets the text inside a block
void Board::setText(RenderWindow &gameBoard, RectangleShape cell, string text, int color, int fontSize)
{
    //create title
    Font font;
    font.loadFromFile(("/System/Library/Fonts/Supplemental/Copperplate.ttc"));
    Text output(text, font, fontSize);
    output.setFillColor(colors[color]);
    output.setOutlineThickness(0.15);
    output.setOutlineColor(Color::Black);

    //center title to middle of square
    FloatRect bounds = output.getLocalBounds();
    output.setOrigin(bounds.left + bounds.width / 2.0f, 
            bounds.top + bounds.height / 2.0f);
    output.setPosition(cell.getPosition().x + cell.getSize().x / 
            2.0f, cell.getPosition().y + cell.getSize().y / 2.0f);

    //return text
    gameBoard.draw(output);
}    

// drawFlag
void Board::drawFlag(RectangleShape &cell)
{
    //load in texture
    Texture texture;
    texture.loadFromFile("/Users/tyganchin/Downloads/MSredFlag.png");

    //create sprite
    Sprite flag;
    flag.setTexture(texture);
    flag.setPosition(cell.getPosition());

    //draw image into the cell
    cell.setTexture(&texture);
    cell.setTextureRect(IntRect(0, 0, cell.getSize().x, cell.getSize().y));
}

// endAnimation
// purpose: executes an ending animation for the game
void Board::endAnimation(RenderWindow &screen, bool won, int scaler, string time, int diff)
{
    //intialize delay
    Clock clock;
    int delayMS = 500;

    //shuffle set and put in a vector
    vector<pair<int, int> > bombLocations(bombLocs.begin(), bombLocs.end());
    random_device rd;
    mt19937 generator(rd());
    shuffle(bombLocations.begin(), bombLocations.end(), generator);

    //set animation to either winning or losing
    Color color = Color::Green;
    SoundBuffer soundBuffer;
    vector<pair<int, int> >::iterator it = bombLocations.begin();
    if (won)
    {
        soundBuffer.loadFromFile("/Users/tyganchin/Downloads/MSbombwon.mp3");
        color = Color::Green;
    }
    else
    {
        soundBuffer.loadFromFile("/Users/tyganchin/Downloads/MSbomblost.mp3");
        color = Color::Red;
        while ((it->first != mineHit.first) or (it->second != mineHit.second))
            ++it;   
    }

    //set sound
    Sound sound;
    sound.setBuffer(soundBuffer);
    sound.setVolume(50);
    
    int count = 0;
    bool first = true;
    vector<pair<int, int> > printLocations;
    while (screen.isOpen())
    {
        Time delay = milliseconds(delayMS);
        Event event;
        while (screen.pollEvent(event))
        {
            //close screen if user closes the window
            if (event.type == sf::Event::Closed)
                screen.close();

            //exit if user clicks
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                    return;     
                else 
                    cout << "right" << endl;           
            }

        }

        // Check if the desired delay has passed
        if ((clock.getElapsedTime() >= delay) or (first))
        {
            first = false;

            // Reset the clock for the next delay
            clock.restart();

            //quit if every bomb has been revealed
            if (count == bombLocs.size())
                return;

            //play sound
            sound.play();

            //reveal mine
            mineField[it->first][it->second].shown = true;
            ++count;

            //add location to print out list
            printLocations.push_back(make_pair(it->first, it->second));

            //iterate to next bomb
            ++it;
            if (it == bombLocations.end())
                it = bombLocations.begin();            
            
            delayMS = max(100, delayMS - 25);
        }

        //draw screen;
        screen.clear(Color::White);
        drawBoard(screen, scaler, time, diff, color);
        screen.display();
    }
}


//test func
void Board::testprint()
{
    for (size_t i = 0; i < mineField.size(); ++i)
    {
        for (size_t j = 0; j < mineField[0].size(); ++j)
        {
            if (mineField[i][j].shown)
                cout << mineField[i][j].val << " ";
            else
                cout << "X" << " ";
        }
        cout << endl;
    }
}