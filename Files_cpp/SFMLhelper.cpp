/*
 * Project: minesweeper
 * Name: SFMLhelper.cpp
 * Author: Tygan Chin
 * Purpose: Implementation of the SFMLhelper class. Defines the functions 
 *          used by a SFMLHelper class to help simplify the use of the SFML
 *          library.
 */

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include "../Files_h/SFMLhelper.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


/******************************************************\
 *                   Text Box Funcs                    *
\******************************************************/

/*
 * makeTextBox
 * Purpose: Create a text box struct given the rectangle and text specifications.
 * Parameters: 
 *      textBoxInfo tboxData : Contains the RectangleShape and Text information
 * Returns: A textBox struct
 * Effects: The text is centered within the rectangle
 */
textBox SFMLhelper::makeTextBox(textBoxInfo tboxData)
{
    return makeTextBox(tboxData.rectangleData, tboxData.textData);
}

/*
 * makeTextBox
 * Purpose: Create a text box struct given the rectangle and text specifications.
 * Parameters:
 *      rectangleInfo rectangleData : struct containing the initialization 
 *                                    information for a RectangleShape
 *                textInfo textData : struct containing the initialization 
 *                                    information for a Text object
 * Returns: A textBox struct
 * Effects: The text is centered within the rectangle
 */
textBox SFMLhelper::makeTextBox(rectangleInfo rectangleData, textInfo textData)
{
    /* create a rectangle and text object */
    Rectangle box = createRec(rectangleData);
    Text text = createText(textData);

    /* center the text in the rectangle */
    centerText(text, box);

    /* return a textBox object */
    textBox tBox;
    tBox.box = box;
    tBox.text = text;
    return tBox;
}

/*
 * drawTextBox
 * Purpose: Draw the text box onto the given renderWindow
 * Parameters: 
 *      RenderWindow &window : The window on the user's screen
 *              textBox &box : A textBox struct containing a RectangleShape
 *                             and a Text object
 * Returns: n/a
 */
void SFMLhelper::drawTextBox(RenderWindow &window, textBox &tBox)
{
    window.draw(tBox.box);
    window.draw(tBox.text);
}


/******************************************************\
 *              RectangleShape Funcs                 *
\******************************************************/

/*
 * createRec
 * Purpose: Initializes and returns a rectangle object using the given data
 * Parameters:
 *      rectangleInfo rectData : struct containing the specifications of the 
 *                               rectangle
 * Returns: A rectangle object
 */
Rectangle SFMLhelper::createRec(rectangleInfo rectData)
{
    return createRec(rectData.width, rectData.height, rectData.xPos, rectData.yPos, rectData.fill, rectData.thickness, rectData.outline);
}

/*
 * createRec
 * Purpose: Initializes and returns a rectangle object using the given data
 * Parameters: 
 *            int width : Width of the rectangle
 *           int height : Height of the rectangle
 *           float xPos : X position of the rectangle in the renderWindow
 *           float yPos : Y position of the rectangle in the renderWindow
 *           Color fill : Color of the inside of the rectangle
 *      float thickness : Thickness of the outline of the rectangle
 *        Color outline : Color of the outline of the rectangle
 * Returns: A rectangle object
 */
Rectangle SFMLhelper::createRec(int width, int height, float xPos, float yPos, Color fill, float thickness, Color outline)
{
    /* create rectangle */
    Rectangle rectangle(Vector2f(width, height));
    rectangle.setPosition(xPos, yPos);
    rectangle.setFillColor(fill);
    rectangle.setOutlineThickness(thickness);
    rectangle.setOutlineColor(outline);

    /* return new rectangle */
    return rectangle;
}


/******************************************************\
 *                   Text Funcs                        *
\******************************************************/

/*
 * createText
 * Purpose: Initializes and returns a text object using the given data
 * Parameters:
 *      textInfo textData : struct containing the specifications of the Text
 *                          object
 * Returns: A text object
 */
Text SFMLhelper::createText(textInfo textData)
{
    return createText(textData.txt, textData.font, textData.fontSize, textData.fill, textData.thickness, textData.outline);
}

/*
 * createText
 * Purpose: Initializes and returns a text object using the given data
 * Parameters:
 *           string txt : The text itself
 *      string fontFile : File the font is in
 *         int fontSize : Size of the text letters
 *           Color fill : Color of the text
 *      float thickness : Outline thickness of the text
 *        Color outline : Outline color of the text
 * Returns: A text object
 */
Text SFMLhelper::createText(string txt, string fontFile, int fontSize, Color fill, float thickness, Color outline)
{
    /* declare sound buffer and load the given file */
    shared_ptr<Font> font = make_shared<Font>();
    font->loadFromFile(fontFile);

    /* add sound buffer to vector to keep it in scope */
    fonts.push_back(font);

    /* create text object */
    Text text(txt, *fonts.back(), fontSize);
    text.setFillColor(fill);
    text.setOutlineThickness(thickness);
    text.setOutlineColor(outline);

    /* return text */
    return text;
}

/*
 * centerText
 * Purpose: Center the given text in the middle of the given rectangle
 * Parameters:
 *                     Text &text : A text object
 *      Rectangle &rectangle : A rectangle object
 * Returns: n/a
 * Effects: The location of the text object changes to be centered
 */
void SFMLhelper::centerText(Text &text, Rectangle &rectangle)
{
    /* get the local bounds of the text */
    FloatRect bounds = text.getLocalBounds();

    /* set the new origin of the text */
    float xOrigin = bounds.left + bounds.width / 2.0f;
    float yOrigin = bounds.top + bounds.height / 2.0f;
    text.setOrigin(xOrigin, yOrigin);

    /* set the new position of the text */
    float xPos = rectangle.getPosition().x + rectangle.getSize().x / 2.0f;
    float yPos = rectangle.getPosition().y + rectangle.getSize().y / 2.0f;
    text.setPosition(xPos, yPos);
} 

/*
 * createText
 * Purpose: Sets the string of a text object and re-centers it within
 *          a given rectangle.
 * Parameters:
 *               string txt : The new text
 *               Text &text : The text object
 *      Rectangle &rec : The rectangle to center text in
 * Returns: A text object
 */
void SFMLhelper::setString(string txt, Text &text, Rectangle &rec)
{
    text.setString(txt);
    centerText(text, rec);
}


/******************************************************\
 *                   Sound Funcs                       *
\******************************************************/

/*
 * setSound
 * Purpose: Initializes and returns a sound object given the soundInfo struct
 * Parameters:
 *      soundInfo soundData : struct containing the necessary information to 
 *                            create the sound object
 * Returns: The initialized sound object
 */
Sound SFMLhelper::setSound(soundInfo soundData)
{
    return setSound(soundData.file, soundData.volume, soundData.pitch, soundData.loop);
}

/*
 * setSound
 * Purpose: Initializes and returns a sound object
 * Parameters: 
 *      string file : File containing the sound
 *       int volume : Volume of the sound
 *      float pitch : Pitch of the sound   
 *        bool loop : Whether the sound loops or not 
 * Returns: The initialized sound object
 */
Sound SFMLhelper::setSound(string file, int volume, float pitch, bool loop)
{
    /* declare sound buffer and load the given file */
    shared_ptr<SoundBuffer> soundBuffer = make_shared<SoundBuffer>();
    soundBuffer->loadFromFile(file);

    /* add sound buffer to vector to keep it in scope */
    buffers.push_back(soundBuffer);

    /* declare sound and intialize it's componets */
    Sound sound;
    sound.setBuffer(*buffers.back());
    sound.setVolume(volume);
    sound.setPitch(pitch);

    /* set whether the sound loops or not */
    sound.setLoop(loop);

    /* return the sound object */
    return sound;
}


/******************************************************\
 *                   Sprite Funcs                      *
\******************************************************/

/* 
 * setSprite
 * purpose: Initialize a Sprite object using the given data
 * arguments:
 *      spriteInfo spriteData : struct containing the information needed to 
 *                              create the sprite object.
 * returns: The initialized sprite object
 */
Sprite SFMLhelper::setSprite(spriteInfo spriteData)
{
    return setSprite(spriteData.file, spriteData.width, spriteData.height, spriteData.xPos, spriteData.yPos);
}

/* 
 * setSprite
 * purpose: Initialize a Sprite object using the given data
 * arguments:
 *      string file : File containing the image
 *        int width : Width of the sprite
 *       int height : Height of the sprite
 *       float xPos : X postion of the sprite within a RenderWindow
 *       float yPos : Y postion of the sprite within a RenderWindow
 * returns: The initialized sprite
 */
Sprite SFMLhelper::setSprite(string file, int width, int height, float xPos, float yPos)
{
    /* declare sound buffer and load the given file */
    shared_ptr<Texture> texture = make_shared<Texture>();
    texture->loadFromFile(file);

    /* add sound buffer to vector to keep it in scope */
    textures.push_back(texture);

    /* create sprite */
    Sprite image;
    image.setTexture(*textures.back());
    image.setPosition(xPos, yPos);
    Vector2f newSize(width, height);

    /* scale sprite to fit desired size */
    float xScale = newSize.x / image.getLocalBounds().width;
    float yScale = newSize.y / image.getLocalBounds().height;
    image.setScale(xScale, yScale);

    /* return sprite */
    return image;
}


/******************************************************\
 *                  User Input Funcs                   *
\******************************************************/

/*
 * leftClick
 * Purpose: Determines if the window event was a left click
 * Parameters:
 *      Event event : The RenderWindow event
 * Returns: True if the left mouse button was clicked, false otherwise
 */
bool SFMLhelper::leftClick(Event event)
{
    return (event.type == Event::MouseButtonPressed) 
            and (event.mouseButton.button == Mouse::Left);
}

/*
 * SpaceBarPressed
 * Purpose: Determines whether the space bar was pressed
 * Parameters:
 *      Event event : The RenderWindow event
 * Returns: True if the space bar was pressed, false otherwise
 */
bool SFMLhelper::SpacebarPressed(Event event)
{
    return (event.type == Event::KeyPressed) 
            and (event.key.code == Keyboard::Space);
}


/******************************************************\
 *                    Time Funcs                       *
\******************************************************/

/* 
 * getTime
 * purpose: Get the current time stored in the given Time object and return 
 *          time in minutes and seconds
 * arguments:
 *      Time time : Time object
 * returns: A string representing the time in minutes and seconds (ex: 01:23)
 */
string SFMLhelper::getTime(Time time)
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
