//
// Tygan Chin
// minesweeper.cpp
//

#include "minesweeper.h"
#include "startingScreen.h"
#include "endingScreen.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;

// Minesweeper
// arguments:n/a
// purpose: Initialize a the minesweeper game
// returns: n/a
Minesweeper::Minesweeper()
{
    //create starting menu and prompt user for difficulty level
    startingScreen menu;
    difficulty = menu.getDifficulty();

    //initilialize board with differenet values depending on the difficulty
    if (difficulty == 1)
        gameBoard.setBoard(8, 8, 10);
    else if (difficulty == 2)
        gameBoard.setBoard(16, 16, 40);
    else   
        gameBoard.setBoard(16, 30, 99);

    //set width and scaler
    if ((difficulty == 1) or (difficulty == 2))
        width = 800;
    else
        width = 1500;

    //set scaler
    if ((difficulty == 2) or (difficulty == 3))
        scaler = 49;
    else   
        scaler = 100;
}

// run
// arguments:n/a
// purpose: runs the game
// returns: n/a
bool Minesweeper::run()
{
    //check if user quit
    if (difficulty == -1)
        return false;

    // create render window
    RenderWindow screen(VideoMode(width, 900), "Board");
    screen.setFramerateLimit(60);

    Clock time;

    //get sound (flag sound, numbered space sound, open space sound)
    SoundBuffer mainBuffer;
    mainBuffer.loadFromFile("/Users/tyganchin/Downloads/MSjumpAudio.wav");
    Sound numbered;
    numbered.setBuffer(mainBuffer);
    numbered.setVolume(50);
    numbered.setPitch(1.0);

    //get sound (flag sound, numbered space sound, open space sound)
    SoundBuffer openBuffer;
    openBuffer.loadFromFile("/Users/tyganchin/Downloads/MSopen.wav");
    Sound openSpace;
    openSpace.setBuffer(openBuffer);
    openSpace.setVolume(50);
    openSpace.setPitch(1.0);

    //get sound (flag sound, numbered space sound, open space sound)
    SoundBuffer flagBuffer;
    flagBuffer.loadFromFile("/Users/tyganchin/Downloads/MSflagDown.mp3");
    Sound flagSound;
    flagSound.setBuffer(flagBuffer);
    flagSound.setVolume(50);
    flagSound.setPitch(1.0);

    // keep reprinting the board, waiting for user input
    bool gameOver = false;
    while (screen.isOpen())
    {
        Event event;
        while (screen.pollEvent(event))
        {
            //check if user exited the window
            if (event.type == sf::Event::Closed)
                screen.close();

            // check if a the mouse was clicked and whether it was a left click
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(screen);
                    if (mousePos.y < 100)
                        continue;
                    
                    int gameState = gameBoard.move((mousePos.y / scaler) 
                                - (100 / scaler), (mousePos.x / scaler));
                    if (gameState == 2) numbered.play();
                    else if (gameState == -1) gameOver = true;
                    else if (gameState == 1) openSpace.play();
                }
            }

            //check if the user clicked their spacebar to assign a flag
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    Vector2i mousePos = Mouse::getPosition(screen);
                    if (mousePos.y < 100)
                        continue;
                    
                    flagSound.play();
                    gameBoard.placeFlag((mousePos.y / scaler) - (100 / scaler), 
                                        (mousePos.x / scaler));
                }
            }
                
        }

        //check if game is over and exit loop if so
        if ((gameOver) or (gameBoard.won()))
        {
            string t = getTime(time.getElapsedTime());
            gameBoard.endAnimation(screen, gameBoard.won(), scaler, t, difficulty);
            screen.close();
            endingScreen end(gameBoard.won(), t, difficulty);
            return end.playAgain();
        }
            
        //draw the screen and display it
        screen.clear(Color::White);
        gameBoard.drawBoard(screen, scaler, getTime(time.getElapsedTime()), difficulty, Color::Transparent);
        screen.display();
    }
    return false;
}

//gets the time of the game
string Minesweeper::getTime(Time time)
{
    // Calculate minutes, seconds, and milliseconds
    int minutes = static_cast<int>(time.asSeconds()) / 60;
    int seconds = static_cast<int>(time.asSeconds()) % 60;

    // Construct the timer string
    stringstream ss;
    ss << setw(2) << minutes << ":" << setfill('0') << setw(2) << seconds;
    return ss.str();   
}