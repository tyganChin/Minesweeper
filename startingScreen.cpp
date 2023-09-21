//
// Tygan Chin
// Purpose: Get the difficulty the user woudld like to play the game with
// startingScreen.cpp
//

#include "startingScreen.h"
#include "SFML/Graphics.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

// startingScreen
// arguments: n/a
// purpose: Create starting menu and get which difficulty user would like
// returns: n/a
startingScreen::startingScreen()
{
    RenderWindow screen(VideoMode(900, 600), "MS menu");

    //set background sound
    SoundBuffer soundBuffer;
    soundBuffer.loadFromFile("/Users/tyganchin/Downloads/MSstart.wav");
    Sound sound;
    sound.setBuffer(soundBuffer);
    sound.setVolume(100);
    sound.setPitch(1.0);
    sound.play();

    //set button sound
    SoundBuffer buttonSoundBuffer;
    buttonSoundBuffer.loadFromFile("/Users/tyganchin/Downloads/MSbutton.mp3");
    Sound button;
    button.setBuffer(buttonSoundBuffer);
    button.setVolume(100);
    button.setPitch(1.0);
    
    while (screen.isOpen())
    {
        Event event;
        while (screen.pollEvent(event))
        {
            //check if user exited the window
            if (event.type == sf::Event::Closed)
            {
                difficulty = -1;
                screen.close();                
            }

            //check if a the mouse was clicked and whether it was a left click
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(screen);
                    if (difficultyChosen(mousePos))
                    {
                        sound.stop();
                        button.play();
                        sleep(seconds(0.25));
                        return;                        
                    }

                }
            }
        }

        //draw the screen and display it
        screen.clear(Color::Black);
        drawTitle(screen);
        drawDifficulties(screen);
        screen.display();
    }

}

// drawTitle
// arguments: the starting menu
// purpose: draw the title of the game at the top of the menu
// returns: n/a
void startingScreen::drawTitle(RenderWindow &screen)
{
    //create rectangle
    RectangleShape rectangle(Vector2f(900, 225));
    rectangle.setPosition(0, 0);
    rectangle.setFillColor(Color::White);
    screen.draw(rectangle);

    //create title
    Font font;
    font.loadFromFile(("/System/Library/Fonts/Supplemental/Copperplate.ttc"));
    Text text("MINESWEEPER", font, 70);
    text.setFillColor(Color::Black);

    //center title to middle of square
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, 
            bounds.top + bounds.height / 2.0f);
    text.setPosition(rectangle.getPosition().x + rectangle.getSize().x / 
            2.0f, rectangle.getPosition().y + rectangle.getSize().y / 2.0f);

    //draw title
    screen.draw(text);  
}

// drawDifficulties
// arguments: The starting menu
// purpose: Draw the difficulty levels on the menu
// returns: n/a
void startingScreen::drawDifficulties(RenderWindow &screen)
{
    // Create rectangle
    RectangleShape rectangle(Vector2f(300, 400));

    //array of pairs to store information for each rectangle
    pair<string, Color> recs[] = {
                make_pair("EASY", Color(0, 200, 0)), 
                make_pair("MEDIUM", Color(255, 255, 0)), 
                make_pair("HARD", Color(200, 0, 0))
    };

    //draw buttons
    for (int i = 0; i < 3; ++i)
    {
        //draw rectangle
        rectangle.setPosition(i * 300, 200);
        rectangle.setFillColor(recs[i].second);
        rectangle.setOutlineThickness(5.0);
        rectangle.setOutlineColor(Color::White);
        screen.draw(rectangle);

        //create title
        Font font;
        font.loadFromFile(("/System/Library/Fonts/Supplemental/Copperplate.ttc"));
        Text text(recs[i].first, font, 30);
        text.setFillColor(Color::Black);


        //center title to middle of square
        FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, 
                bounds.top + bounds.height / 2.0f);
        text.setPosition(rectangle.getPosition().x + rectangle.getSize().x / 
                2.0f, rectangle.getPosition().y + rectangle.getSize().y / 2.0f);

        //draw title
        screen.draw(text);        
    }
}

// difficultyChosen
// arguments: user's mouse position when clicked
// purpose: determine which difficulty the user picked (if any)
// returns: true if difficulty was picked, false otherwise
bool startingScreen::difficultyChosen(Vector2i &mousePos)
{
    //check if click was on title box
    if (mousePos.y <= 200)
        return false;
    
    if (mousePos.x <= 300) //easy
        difficulty = 1;
    else if (mousePos.x > 600) //hard
        difficulty = 3;
    else //medium
        difficulty = 2;
    
    return true;
}

// getDifficulty
// arguments: n/a
// purpose: get the difficulty the user picked
// returns: difficulty (1 = easy, 2 = medium, 3 = hard)
int startingScreen::getDifficulty()
{
    return difficulty;
}