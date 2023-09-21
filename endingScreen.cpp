// endingScreen.cpp
// Tygan Chin
// Purpose: Display game info to user and get whether they want to play again
// endingScreen.cpp
//

#include "endingScreen.h"
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

// endingScreen
// arguments: n/a
// purpose: Create an ending menu and get if they want to play again
// returns: n/a
endingScreen::endingScreen(bool gameWon, string time, int &difficulty)
{
    //create window
    RenderWindow screen(VideoMode(900, 600), "Ending Screen");

    //set background sounds and image
    SoundBuffer musicB, soundEffect;
    Texture texture;
    if (gameWon)
    {
        musicB.loadFromFile("/Users/tyganchin/Downloads/MSwin.mp3");   
        soundEffect.loadFromFile("/Users/tyganchin/Downloads/MSwinSE.wav");  
        texture.loadFromFile("/Users/tyganchin/Downloads/MSwinimage.png"); 
    }
    else
    {
        musicB.loadFromFile("/Users/tyganchin/Downloads/MSloss.mp3");
        soundEffect.loadFromFile("/Users/tyganchin/Downloads/MSlossSE.mp3");  
        texture.loadFromFile("/Users/tyganchin/Downloads/MSbombimg.png");
    }

    //intialize image
    Sprite ogImage;
    ogImage.setTexture(texture);
    ogImage.setPosition(200, 0);
    Vector2f newSize(500, 500);
    ogImage.setScale(newSize.x / ogImage.getLocalBounds().width,
            newSize.y / ogImage.getLocalBounds().height);

    //intialize delay
    Clock clock;
    Time delay = milliseconds(3000);

    Sound effect;
    effect.setBuffer(soundEffect);
    effect.setVolume(100);
    effect.setPitch(1.25);
    effect.play();

    Sound music;
    music.setBuffer(musicB);
    music.setVolume(75);

    //set button sound
    SoundBuffer buttonSoundBuffer;
    buttonSoundBuffer.loadFromFile("/Users/tyganchin/Downloads/MSbutton.mp3");
    Sound button;
    button.setBuffer(buttonSoundBuffer);
    button.setVolume(100);
    button.setPitch(1.0);

    //get the old time
    getOldTime(difficulty);

    bool first = true;
    while (screen.isOpen())
    {
        Event event;
        while (screen.pollEvent(event))
        {
            //check if user exited the window
            if (event.type == sf::Event::Closed)
                screen.close();

            //check if a the mouse was clicked and whether it was a left click
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(screen);
                    if (decision(mousePos))
                    {
                        //play sound effects;
                        music.stop();
                        effect.stop();
                        button.play();
                        sleep(seconds(0.25));
            
                        return;
                    }
                }
            }
        }

        //draw the screen and display it
        screen.clear(Color::White);

        // screen.draw(modImage);
        screen.draw(ogImage);
        
        //won or lost
        if (gameWon)
            drawSquare(screen, 500, 900, 0, 0, Color::Transparent, "YOU WON", 165, Color::Green);
        else
            drawSquare(screen, 500, 900, 0, 0, Color::Transparent, "YOU LOST", 165, Color::Red);     


        //difficulty
        pair <string, Color> diffs[] = {make_pair("E", Color::Green),
                                        make_pair("M", Color::Yellow),
                                        make_pair("H", Color::Red)};
        drawSquare(screen, 100, 100, 800, 0, Color::Transparent, 
                diffs[difficulty - 1].first, 80, diffs[difficulty - 1].second);

        //draw times
        bestTime(screen, time, gameWon, difficulty);

        //quit or continue
        drawSquare(screen, 80, 200, 0, 520, Color::Red, "QUIT", 30, Color::White); 
        drawSquare(screen, 80, 200, 700, 520, Color::Green, "PLAY AGAIN", 30, Color::White); 

        screen.display();

         // Check if the desired delay has passed
        if ((clock.getElapsedTime() >= delay) and (first))
        {
            effect.stop();
            first = false;
            music.play();
        }
    }
}

// drawSquare
// arguments: the starting menu
// purpose: draw the title of the game at the top of the menu
// returns: n/a
void endingScreen::drawSquare(RenderWindow &screen, int height, int width, int x, int y, Color backgroundColor, string output, int fontSize, Color textColor)
{
    //create rectangle
    RectangleShape rectangle(Vector2f(width, height));
    rectangle.setPosition(x, y);
    rectangle.setFillColor(backgroundColor);
    screen.draw(rectangle);

    //create title
    Font font;
    font.loadFromFile(("/System/Library/Fonts/Supplemental/Copperplate.ttc"));
    Text text(output, font, fontSize);
    text.setFillColor(textColor);
    text.setOutlineColor(Color::Black);
    text.setOutlineThickness(0.5);

    //center title to middle of square
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, 
            bounds.top + bounds.height / 2.0f);
    text.setPosition(rectangle.getPosition().x + rectangle.getSize().x / 
            2.0f, rectangle.getPosition().y + rectangle.getSize().y / 2.0f);

    //draw title
    screen.draw(text);
}

bool endingScreen::decision(Vector2i &mousePos)
{
    //check if click was on title box
    if (mousePos.y <= 520)
        return false;
    
    if (mousePos.x <= 200) //play again 
        again = false;
    else if (mousePos.x > 600)
        again = true;
    else
        return false;
    
    return true;
}


bool endingScreen::playAgain()
{
    return again;
}

// 
void endingScreen::getOldTime(int &difficulty)
{
    //open file
    if (difficulty == 1)
        filename = "/Users/tyganchin/Documents/projects/minesweeper/bestEasy.txt";
    else if (difficulty == 2)
        filename = "/Users/tyganchin/Documents/projects/minesweeper/bestMedium.txt";
    else   
        filename = "/Users/tyganchin/Documents/projects/minesweeper/bestHard.txt";
    ifstream file(filename);
    
    //read in old time
    file >> oldTime;
    file.close();
}

// bestTime 
// prints out the time of the user if they won and whether or not they beat the fastest time
void endingScreen::bestTime(RenderWindow &screen, string &time, bool &gameWon, int &difficulty)
{
    //draw old time
    drawSquare(screen, 250, 250, 500, 310, Color::Transparent, "Best: " + 
        oldTime, 50, Color::Black);

    if (not gameWon)
    {
        drawSquare(screen, 250, 250, 145, 310, Color::Transparent, "...N/A...", 50, Color::Black);
    }
    else if (smaller(time, oldTime))
    {
        drawSquare(screen, 250, 250, 150, 310, Color::Transparent, "Time: " + time, 50, Color::Green);

        //update best time file
        ofstream output(filename);
        output << time;
        output.close();
    }
    else
    {
        drawSquare(screen, 250, 250, 150, 310, Color::Transparent, "Time: " + time, 50, Color::Black);
    }
}

// smaller
// returns whether the new time is faster than the fastest time
bool endingScreen::smaller(string &time, string &oldTime)
{
    //account for blank spaces
    int oldInd = 0, currInd = 0;
    while (oldTime[oldInd] == ' ' or oldTime[oldInd] == '0')
        ++oldInd;
    while (time[currInd] == ' ' or time[currInd] == '0')
        ++currInd;
    
    //check if the lengths differ (no need to compare individual numbers)
    if (time.length() - currInd < oldTime.length() - oldInd)
        return true;
    else if (time.length() - currInd > oldTime.length() - oldInd)
        return false;
    
    //compare each number
    for (int i = 0; i < oldTime.length(); ++i)
    {
        if (time[i + currInd] < oldTime[i + oldInd])
            return true;
        else if (time[i + currInd] > oldTime[i + oldInd])
            return false;
    }
    
    //return true if they are equal
    return true;
}