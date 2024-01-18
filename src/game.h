#ifndef GAME_H
#define GAME_H
#include <iostream>
/*

Ici on a tous les headers et les constantes utilisées dans le programme

*/

#define COLUMNS 7       // Nombre de colonne de la grill intérieur
#define ROWS 8          // Nombre de ligne  de la grill intérieur
#define STARTER_LINES 2 // Le nombre de linge au départ
#define STARTER_MAX 6   // Le nombre d'une case max
#define STARTER_MIN 1   // Le nombre d'une case minimum

// Paramètres de la taille de la fenetre

using namespace std;

#define windowWidth 500
#define windowHeigth 600
#define unitWidthNumber 20                        // Nombre de colonne de la grill intérieur
#define unitHeigthNumber 25                       // Nombre de Ligne de la grill intérieur
#define unitWidth (windowWidth / unitWidthNumber) // Conversion
#define unitHeight (windowHeigth / unitHeigthNumber)

#define OFFSET 3 // offset pour la grille intérieur
#define UNIT 2   // taille du petit carré à l'interieur de la grille

#define TIMERSPEED 1 // Game timer inceremented width per seconde (from 0  to 100)
// #define TIMERSPEED 50 // for debugging

#define FPS 10
#define PAGEREFRESH 1000 / FPS // Game speed (Number of refreshed screen in 1 seconde)
// #define PAGEREFRESH 2000 // for debugging

#define PLAY_MUSIC_ON true     // Enable or Disable all game musics
#define GAMEOVER_ON true       // Enable or Disable GameOver feature
#define GAMEOVER_LINE ROWS  // The max number rows before showing the GameOver (from 2 to ROWS)
// #define GAMEOVER_LINE 3 // for debugging

/*
    structure Square contenant plusieurs attributs qui décrivent les caractéristiques du carré (sa position, s'il est affiché, son chiffre et sa couleur)
*/
struct Square
{
    int index;         // index in Matrix (form 0 to ROWS*COLUMS )
    float x, y;        // pixel position in the Matrix
    int number;        // the number shown in the square
    std::string color; // the color of the suare in the matrix
    bool isMoving;
};

#endif // GAME_H
