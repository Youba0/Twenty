#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include "game.h"
#include "events.cpp"
#include <vector>
#include <array>
#include <string>
#include "ui_component.cpp"

using namespace std;

/*
    Variable de psositionement
*/
float mouseX, mouseY;
int squareIndex;
int oldSquareY;

float timerBarWidth = 0; // Initial width of the timer bar
int maxDisplayedLine = 0;

// start button positioning
float btnStartX = 15;
float btnStartY = 1;
float buttonSizeX = 4; // Largeur du bouton
float buttonSizeY = 1; // Hauteur du bouton
bool gameOver = false;
int score = 0;
/*
Liste de 20 couleurs en hexadécimal
*/
std::array<std::string, 20> beautifulColors = {
    "#FF6347", // Tomato
    "#4682B4", // Steel Blue
    "#2E8B57", // Sea Green
    "#FFD700", // Gold
    "#BA55D3", // Medium Orchid
    "#00FFFF", // Cyan
    "#8A2BE2", // Blue Violet
    "#32CD32", // Lime Green
    "#FF69B4", // Hot Pink
    "#1E90FF", // Dodger Blue
    "#F08080", // Light Coral
    "#ADFF2F", // Green Yellow
    "#20B2AA", // Light Sea Green
    "#FFA07A", // Light Salmon
    "#9932CC", // Dark Orchid
    "#00FF00", // Green
    "#B0E0E6", // Powder Blue
    "#FF8C00", // Dark Orange
    "#DDA0DD", // Plum
    "#7FFF00"  // Chartreuse
};

/*
    Variable de controle
*/

std::vector<Square> squares; // Stockage des carrés

bool isDragging = false;
bool isMerging = false;
bool isAddingNewRow = false;

/*
La fonction principale du jeu, elle fait l'initialisation des sqaures au début du jeu, drawbackground, timerbar, 
drawgrid, score, la gravité, et le gameover
*/
void mainLoop()
{

    if (squares.empty())
    {
        initSquares(squares, ROWS * COLUMNS);
    }

    // cout << "Screen updated \n";
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    displayTimerBar(timerBarWidth);

    int max = score; // initialisation du score
    for (int i = 0; i < ROWS * COLUMNS; i++)
    {

        int squareIndex = findSqaureWithIndex(squares, i);
        if (squareIndex != -1)
        {
            Square square = squares[squareIndex];
            
            if (!isDragging && !isMerging)
            {
                square = updateSquareIndex(squares, squareIndex, oldSquareY); // Gravity down
            }
            max = max < square.number ? square.number : max;
            drawSquare(square, isMerging, canOnlyBeMergin);
        }
    }
    disaplyScore(max);
    displayStartButton(btnStartX, btnStartY);
    drawGrid(); // dessine la grille

    if (maxDisplayedLine == GAMEOVER_LINE)
    {
        if (GAMEOVER_ON)
        {
            displayGameOver();
            if (!gameOver)     //Pour s'assurer que la musique se lance une fois 
            {
                playGameOverMusic();
                gameOver = true;
            }
        }
        timerBarWidth = 0;
    }
    else if (timerBarWidth >= 100)
    {
        timerBarWidth = 0;
        isDragging = false;
        isMerging = false;
        add_new_row(squares, max, maxDisplayedLine);
        cout << "max displayed line: " << maxDisplayedLine << "\n";
    }

    glutSwapBuffers(); // glut
}

/*
 détécte quel square on a sellectionner et ou on l'a déposer, et restart

*/
void mouseClick(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y; // Inverser la position Y
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        squareIndex = clickOnSquare(squares,
                                    mouseX, mouseY,
                                    isDragging, isMerging,
                                    oldSquareY);
        printMatrix(squares);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        releaseSquare(squares,
                      mouseX, mouseY,
                      squareIndex,
                      oldSquareY,
                      isDragging, isMerging);
        squareIndex = -1;
        printMatrix(squares);
    }

    float realBtnX = btnStartX * unitWidth;                   // La position exacte sur la fentre pour le bouton start
    float realBtnY = windowHeigth - (btnStartY * unitHeight); // because positision Y is inversed
    float realBtnXSize = buttonSizeX * unitWidth;
    float realBtnYSize = buttonSizeY * unitHeight;

    // Check if the left mouse button is pressed and if it's in the button area
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x >= realBtnX && x <= realBtnX + realBtnXSize &&
        y <= realBtnY && y >= realBtnY - realBtnYSize)
    {
        // Perform actions when the button is clicked
    
        restart_game(squares, timerBarWidth, maxDisplayedLine);
    }
}


/*
Fonction pour déplacer le square avec le x et y de la souris
*/
void mouseMotion(int x, int y)
{
    if (isDragging || isMerging) //Si on peut déplacer ou fusionner un square 
    {
        std::cout << "Button clicked!\n";

        mouseX = x;
        mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y; // Inverser la position Y
        dragSquare(squares, mouseX, mouseY, squareIndex, isDragging, isMerging);
    }
}
