/*
 * Project: minesweeper
 * Name: SFMLhelper.cpp
 * Author: Tygan Chin
 * Purpose: Helper functions for the SMFL library
 */

#ifndef SFMLHELPER_H
#define SFMLHELPER_H

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/* text box struct used for drawing */
struct textBox {
    RectangleShape box;
    Text text;
};

/* scaling rectangle struct */
struct rectangleInfo {
    int width, height;
    float xPos, yPos;
    Color fill;
    float thickness;
    Color outline;
};

/* scaling text struct */
struct textInfo {
    string txt;
    string font;
    int fontSize;
    Color fill;
    float thickness;
    Color outline;
};

/* text box struct used to store the textBox initializing data */
struct textBoxInfo {
    rectangleInfo rectangleData;
    textInfo textData;
};

/* struct to initialize a sound object */
struct soundInfo {
    string file;
    int volume;
    float pitch;
    bool loop;
};

/* struct to initialize a sprite (image) */
struct spriteInfo {
    string file;
    float width, height;
    float xPos, yPos;
};

/* colors */
static const Color WHITE        = Color::Red;
static const Color BLACK        = Color::Black;
static const Color RED          = Color::Red;
static const Color BLUE         = Color::Blue;
static const Color GREEN        = Color::Green;
static const Color YELLOW       = Color::Yellow;
static const Color CYAN         = Color::Cyan;
static const Color MAGENTA      = Color::Magenta;
static const Color TRANSPARENT  = Color::Transparent;

/* helper functions */
void centerWindow(RenderWindow &window, int width, int height);
textBox makeTextBox(textBoxInfo textBoxData);
RectangleShape createRec(rectangleInfo rectData);
Text createText(textInfo textData);
void centerText(Text &text, RectangleShape &rectangle);
Sound setSound(soundInfo soundData);
Sprite setSprite(spriteInfo spriteData);
bool leftClick(Event event);
bool SpacebarPressed(Event event);
string getTime(Time time);
void drawTextBox(RenderWindow &window, textBox &tBox);

#endif
