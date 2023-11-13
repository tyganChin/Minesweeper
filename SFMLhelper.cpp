/*
 * Project: minesweeper
 * Name: SFMLhelper.cpp
 * Author: Tygan Chin
 * Purpose: Helper functions for the SMFL library
 */

#include <string>
#include <sstream>
#include <iomanip>
#include "SFMLhelper.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


/******************************************************\
 *                   Window Funcs                      *
\******************************************************/

/*
 * makeWindow
 * Purpose: Initialize a render window and center it in the users screen using
 *          the given width and height
 * Parameters: The width and height of the render window and it's name
 * Returns: The initialized renderwindow
 */
void centerWindow(RenderWindow &window, int width, int height)
{    
    /* get size of user's window */
    int screenWidth = VideoMode::getDesktopMode().width;
    int screenHeight = VideoMode::getDesktopMode().height;

    /* center the window in the users screen */
    float xPos = (screenWidth - width) / 2;
    float yPos = (screenHeight - height) / 2;
    window.setPosition(Vector2i(xPos, yPos));
}


/******************************************************\
 *                     Text Boxes                      *
\******************************************************/

/*
 * makeTextBox
 * Purpose: Create a text box struct given the rectangle and text specifications.
 * Parameters: A textBoxInfo struct containing the box and text information
 * Returns: A textBox struct
 * Notes: The text is centered within the rectangle
 */
textBox makeTextBox(textBoxInfo textBoxData)
{
    /* create a rectangle and text object */
    RectangleShape box = createRec(textBoxData.rectangleData);
    Text text = createText(textBoxData.textData);

    /* center the text in the rectangle */
    centerText(text, box);

    /* return a textBox object */
    return ((textBox){box, text});
}

/*
 * createRec
 * Purpose: Initializes and returns a rectangle object using the given data
 * Parameters: A Rectangle struct containing the specifications of the 
 *             rectangle
 * Returns: A rectangle object
 */
RectangleShape createRec(rectangleInfo rectData)
{
    /* create rectangle */
    RectangleShape rectangle(Vector2f(rectData.width, rectData.height));
    rectangle.setPosition(rectData.xPos, rectData.yPos);
    rectangle.setFillColor(rectData.fill);
    rectangle.setOutlineThickness(rectData.thickness);
    rectangle.setOutlineColor(rectData.outline);

    /* return new rectangle */
    return rectangle;
}

/*
 * createText
 * Purpose: Initializes and returns a text object using the given data
 * Parameters: A Text struct containing the specifications of the text
 * Returns: A text object
 */
Text createText(textInfo textData)
{
    /* load in font */
    Font textFont;
    textFont.loadFromFile(textData.txt);

    /* create text object */
    Text text(textData.txt, textFont, textData.fontSize);
    text.setFillColor(textData.fill);
    text.setOutlineThickness(textData.thickness);
    text.setOutlineColor(textData.outline);

    /* return text */
    return text;
}

/*
 * centerText
 * Purpose: Center the given text in the middle of the given rectangle
 * Parameters: A text and rectangle object
 * Returns: n/a
 * Effects: The location of the text object changes to be centered
 */
void centerText(Text &text, RectangleShape &rectangle)
{
    /* set the new origin of the text */
    FloatRect bounds = text.getLocalBounds();
    float xOrigin = bounds.left + bounds.width / 2.0f;
    float yOrigin = bounds.top + bounds.height / 2.0f;
    text.setOrigin(xOrigin, yOrigin);

    /* set the new position of the text */
    float xPos = rectangle.getPosition().x + rectangle.getSize().x / 2.0f;
    float yPos = rectangle.getPosition().y + rectangle.getSize().y / 2.0f;
    text.setPosition(xPos, yPos);
} 

/*
 * drawTextBox
 * Purpose: 
 * Parameters: 
 * Returns: 
 */
void drawTextBox(RenderWindow &window, textBox &tBox)
{
    window.draw(tBox.box);
    window.draw(tBox.text);
}

/******************************************************\
 *                       Sound                         *
\******************************************************/

/*
 * setSound
 * Purpose: Initializes and returns a sound object given the soundInfo struct
 * Parameters: A soundInfo struct containing the necessary information to 
 *             create the sound
 * Returns: The initialized sound
 */
Sound setSound(soundInfo soundData)
{
    /* declare sound buffer and load the given file */
    SoundBuffer soundBuffer;
    soundBuffer.loadFromFile(soundData.file);

    /* declare sound and intialize it's componets */
    Sound sound;
    sound.setBuffer(soundBuffer);
    sound.setVolume(soundData.volume);
    sound.setPitch(soundData.pitch);

    /* set whether the sound loops or not */
    sound.setLoop(soundData.loop);

    /* return the sound object */
    return sound;
}

/******************************************************\
 *                       Sprites                       *
\******************************************************/

/* 
 * setSprite
 * purpose: Initialize a Sprite object using the given data
 * arguments: A spriteInfo struct containing the information needed to create
 *            a sprite.
 * returns: The initialized sprite
 */
Sprite setSprite(spriteInfo spriteData)
{
    /* load image */
    Texture texture;
    texture.loadFromFile(spriteData.file);

    /* create sprite */
    Sprite image;
    image.setTexture(texture);
    image.setPosition(spriteData.xPos, spriteData.yPos);
    Vector2f newSize(spriteData.width, spriteData.height);

    /* scale sprite to fit desired size */
    float xScale = newSize.x / image.getLocalBounds().width;
    float yScale = newSize.y / image.getLocalBounds().height;
    image.setScale(xScale, yScale);

    /* return sprite */
    return image;
}

/******************************************************\
 *                     User input                      *
\******************************************************/

/*
 * leftClick
 * Purpose: Determines if the window event was a left click
 * Parameters: The event
 * Returns: True if the left mouse button was clicked, false otherwise
 */
bool leftClick(Event event)
{
    return (event.type == Event::MouseButtonPressed) 
            and (event.mouseButton.button == Mouse::Left);
}

/*
 * SpaceBarPressed
 * Purpose: Determines whether the space bar was pressed
 * Parameters: A window event
 * Returns: True if the space bar was pressed, false otherwise
 */
bool SpacebarPressed(Event event)
{
    return (event.type == Event::KeyPressed) 
            and (event.key.code == Keyboard::Space);
}

/******************************************************\
 *                       Time                          *
\******************************************************/

/* 
 * getTime
 * purpose: Get the current time stored in the given Time object and return 
 *          time in minutes and seconds
 * arguments: A time object
 * returns: A string of the time
 */
string getTime(Time time)
{
    /* Calculate minutes and seconds */
    int minutes = static_cast<int>(time.asSeconds()) / 60;
    int seconds = static_cast<int>(time.asSeconds()) % 60;

    /* Construct the timer string */
    stringstream ss;
    ss << setw(2) << minutes << ":" << setfill('0') << setw(2) << seconds;

    /* return the string */
    return ss.str();   
}
