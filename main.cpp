#include <GL/gl.h>
#include <GL/glut.h>
#include "./src/game.h" //pour inclure les headers
#include "./src/game.cpp"

#include <iostream>
#include <atomic>

using namespace std;

void reshape_callback(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, unitWidthNumber, 0.0, unitHeigthNumber, -1.0, 1.0); // taille de la grille externe
    glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int)
{
    if (maxDisplayedLine < GAMEOVER_LINE)
    {
        timerBarWidth += TIMERSPEED;
    }

    glutPostRedisplay();
    glutTimerFunc(PAGEREFRESH, timer_callback, 0);
}

/*
Code principale
*/
int main(int argc, char **argv)
{

    int width = windowWidth;
    int heigth = windowHeigth;

    glutInit(&argc, argv);                       
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); 
    glutInitWindowSize(width, heigth);           // window size

    glutCreateWindow("Twenty!_by Youba"); // titre de la fenetre
    setBackgroundImage(width, heigth);
    glutDisplayFunc(mainLoop); //Le jeu 

    glutReshapeFunc(reshape_callback); 
    glutTimerFunc(0, timer_callback, 0);
    glutMouseFunc(mouseClick); // clickOnSquare and Release square 
    glutMotionFunc(mouseMotion); // is possible dragSquare
    playBackGroundMusic();
    glutMainLoop(); // glut boocle

    return 0;
}

