#include <iostream>
#include <sstream> //bibliothèque pour manipuler des strings
#include <iomanip>
#include <array>
#include <vector>
#include <stdexcept>
#include "game.h"

int get_x(int index)
{
    return (index % COLUMNS) * 2;
}

int get_y(int index)
{
    return (index / COLUMNS) * 2;
}

int get_line(int yPixel)
{
    return yPixel / UNIT;
}

int get_column(int xPixel)
{
    return xPixel / UNIT;
}

int get_mouse_column(int mouseX)
{
    return ((mouseX / unitWidth) - OFFSET) / UNIT;
}

int get_mouse_line(int mouseY)
{
    return ((mouseY / unitHeight) - OFFSET) / UNIT;
}

float to_pixel(int index)
{
    return index * UNIT;
}

int get_index(int x, int y)
{
    return y * (ROWS - 1) + x;
}

/*
    Renvoie un nombre aléatoire entre min et max
*/
int random(int max, int min)
{
    return min + rand() % (max - min);
}

/*
 Renvoie une liste de numéro random 
*/
int *list_random_numbers(int size, int max, int min)
{

    int *dynamicArray = new int[size]; // Creates a dynamic array of 10 integers

    // Accessing and modifying elements

    for (int i = 0; i < size; i++)
    {
        dynamicArray[i] = random(max, min);
    }

    return dynamicArray;
}

extern std::array<std::string, 20> beautifulColors;

Square getSquare(int index, float x, float y, int number, bool isMoving = false)
{
    std::string color = beautifulColors[number];
    cout << "Create Square with number" << number << " in position " << index << " (x: " << x << " , y: " << y << " )\n";
    return {index, x, y, number, color, isMoving};
}

/*
    Pose les squares initiales aléatoirement
*/
void initSquares(std::vector<Square> &squares, int size)
{
    squares.clear();
    // int a random list as a global variable, used as the starting squares
    int *random_starter = list_random_numbers(size, STARTER_MAX, STARTER_MIN);

    for (int i = 0; i < size; i++)
    {
        float y = (i / COLUMNS);
        float x = (i % COLUMNS);

        if (i < STARTER_LINES * COLUMNS)
        {
            Square new_square = getSquare(i, to_pixel(x), to_pixel(y), random_starter[i]);
            squares.push_back(new_square);
        }
    }
}