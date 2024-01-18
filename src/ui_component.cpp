
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include "game.h"

using namespace std;
extern float buttonSizeX;
extern float buttonSizeY;

// private

// Function to convert hexadecimal color string to float RGB values
void hexToRgb(const std::string &hexColor, float &r, float &g, float &b)
{
    if (hexColor.size() != 7 || hexColor[0] != '#')
    {
        throw std::invalid_argument("Invalid hexadecimal color format");
    }

    std::stringstream ss;
    ss << std::hex << hexColor.substr(1);

    int rgbValue;
    ss >> rgbValue;

    // Extract individual R, G, B components and normalize to [0.0, 1.0]
    r = ((rgbValue >> 16) & 0xFF) / 255.0f;
    g = ((rgbValue >> 8) & 0xFF) / 255.0f;
    b = (rgbValue & 0xFF) / 255.0f;
}

void set_color(string hex_color)
{
    float red, green, blue;
    try
    {
        hexToRgb(hex_color, red, green, blue);
        glColor3f(red, green, blue);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
/*
    Dessine un carré au position x et y avec une hauteur et une largeur indiquées, et une couleur en hexadécimal
*/
void drawSquareShape(float x, float y, float width, float height, string hex_color = "", bool drawBorder = false, string border_color = "")
{
    // Dessiner la barre des scores
    glLineWidth(1.0);
    int type = GL_LINE_LOOP;
    if (hex_color == "")
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    else
    {
        type = GL_QUADS;
        set_color(hex_color);
    }

    glBegin(type);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);

    glEnd();

    // Draw the red border if requested
    if (drawBorder)
    {
        glLineWidth(4.0);
        set_color(border_color);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }
}

/*
    écrit un text au position x et y indiqués
*/
void drawText(float x, float y, string text, int color = 1)
{
    if (color == 0)
    {
        glColor3f(0, 0, 0);
    }
    else
    {
        glColor3f(1, 1, 1);
    }

    glRasterPos2f(x, y);
    for (char &c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawSquare(Square square, bool isMerging, bool canOnlyBeMergin)

{
    int x = square.x;
    int y = square.y;
    int number = square.number;
    string color = square.color;

    int reelX = x + OFFSET;
    int reelY = y + OFFSET;
    // Calculate the center of the square
    float centerX = reelX + 0.9;
    float centerY = reelY + 0.77;

    // Convert the number to a string
    string numberString = std::to_string(number);

    // Offset the text position to center it within the square
    float textOffsetX = centerX - 0.08 * numberString.length();
    float textOffsetY = centerY;

    // Draw the number at the center of the square

    float smallBoxX = reelX + 0.15;
    float smallBoxY = reelY + 0.15;
    string borderColor;

    if (isMerging)
    {
        borderColor = "#65B741";
    }
    else if (canOnlyBeMergin)
    {
        borderColor = "#FFFFFF";
    }
    else
    {
        borderColor = "#FF0000";
    }

    drawSquareShape(smallBoxX, smallBoxY, 1.7, 1.7, color, square.isMoving, borderColor);
    drawText(textOffsetX, textOffsetY, numberString);
}

// public
void displayTimerBar(float width)
{
    drawSquareShape(3, 19.3, (COLUMNS * UNIT) * (width / 100), 0.25, "#00FF00");
}
void disaplyScore(int max)
{

    drawSquareShape(13, 21, buttonSizeX, buttonSizeY, "#F08080"); // dessine le réctangle du score
    drawText(13.3, 21.3, "Score :  " + to_string(max));           // affiche le score
}

void displayStartButton(float btnStartX, float btnStartY, std::string text = "Start")
{
    drawSquareShape(btnStartX, btnStartY, buttonSizeX, buttonSizeY); // dessine le réctangle du bouton start
    drawText(btnStartX + 1, btnStartY + 0.3, text, 1);
}

void displayGameOver()
{
    drawSquareShape(4, 7, 12, 8, "#7E2553"); // dessine le réctangle du bouton start
    drawText(8, 11, "Game Over", 1);
}

/*
 * dessiner un petit carré dans la grille
 */
void unit(int x, int y)
{

    glLineWidth(1.0);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + UNIT, y);
    glVertex2f(x + UNIT, y + UNIT);
    glVertex2f(x, y + UNIT);
    glEnd();
}

/*
 * Draw a grid using the global variables
 */
void drawGrid()
{

    for (int i = 0; i < COLUMNS * 2; i = i + 2)
    {
        for (int j = 0; j < ROWS * 2; j = j + 2)
        {
            unit(i + OFFSET, j + OFFSET);
        }
    }
}