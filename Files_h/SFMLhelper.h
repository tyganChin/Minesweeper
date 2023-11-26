/*
 * Project: minesweeper
 * Name: SFMLhelper.h
 * Author: Tygan Chin
 * Purpose: Interface for SFMLhelper class. Declares the functions used by 
 *          a SFMLHelper instance to help simplify the use of the SFML 
 *          library.
 */

#ifndef SFMLHELPER_H
#define SFMLHELPER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace sf;

/* types to help simplify the readablity of the SFML libarary */
typedef RenderWindow RW;
typedef RectangleShape Rectangle;

/* text box struct used for drawing */
struct textBox {
    Rectangle box;
    Text text;
};

/* contains the necessary information to initialize a RectangleShape object */
struct rectangleInfo {
    int width, height;
    float xPos, yPos;
    Color fill;
    float thickness;
    Color outline;
};

/* contains the necessary information to initialize a Text object */
struct textInfo {
    string txt;
    string font;
    int fontSize;
    Color fill;
    float thickness;
    Color outline;
};

/* contains the necessary information to initialize a textBox struct */
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
static const Color WHITE        = Color::White;
static const Color BLACK        = Color::Black;
static const Color RED          = Color::Red;
static const Color BLUE         = Color::Blue;
static const Color GREEN        = Color::Green;
static const Color YELLOW       = Color::Yellow;
static const Color CYAN         = Color::Cyan;
static const Color MAGENTA      = Color::Magenta;
static const Color TRANSPARENT  = Color::Transparent;

/* delay length (in seconds) for a button sound */
static const float BUTTON_DELAY = 0.25;

class SFMLhelper 
{
public: 

    /* TextBox Funcs */
    textBox makeTextBox(textBoxInfo tboxData);
    textBox makeTextBox(rectangleInfo rectangleData, textInfo textData);
    void drawTextBox(RenderWindow &window, textBox &tBox);

    /* RectangleShape Funcs */
    Rectangle createRec(rectangleInfo rectData);
    Rectangle createRec(int width, int height, float xPos, float yPos, Color fill, float thickness, Color outline);

    /* Text Funcs */
    Text createText(textInfo textData);
    Text createText(string txt, string fontFile, int fontSize, Color fill, float thickness, Color outline);
    void centerText(Text &text, Rectangle &rectangle);
    void setString(string txt, Text &text, Rectangle &rec);

    /* Sound Funcs */
    Sound setSound(soundInfo soundData);
    Sound setSound(string file, int volume, float pitch, bool loop);

    /* Sprite Funcs */
    Sprite setSprite(spriteInfo spriteData);
    Sprite setSprite(string file, int width, int height, float xPos, float yPos);

    /* User Input Funcs */
    bool leftClick(Event event);
    bool SpacebarPressed(Event event);

    /* Time Funcs */
    string getTime(Time time);

private:

    /*
     *  Prolongs the scope of the files containing the sounds, images, and fonts
     *  used in the program 
     */
    vector<shared_ptr<SoundBuffer> > buffers;
    vector<shared_ptr<Font> > fonts;
    vector<shared_ptr<Texture> > textures;

};

#endif
