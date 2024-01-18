#include "game.h"
#include "helpers.cpp"
#include "assets.cpp"

#include <sstream> //bibliothèque pour manipuler des strings
#include <iomanip>
#include <stdexcept>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>

void add_new_row(std::vector<Square> &squares, int max, int &maxLine)
{
    cout << "Add new row \n";
    // move current squares up
    for (Square &square : squares)
    {
        if (square.index != -1)
        {
            cout << "Raise square with number" << square.number << " of one box up from " << get_line(square.y) << " to " << get_line(square.y) + 1 << " \n";

            int new_line = get_line(square.y) + 1;

            if (maxLine < new_line)
            {
                maxLine = new_line;
            }

            square.y = to_pixel(new_line);
            square.index = get_index(get_column(square.x), new_line);
        }
    }

    int *random_starter = list_random_numbers(COLUMNS, max, STARTER_MIN);

    // add new row
    for (int i = 0; i < COLUMNS; i++)
    {
        int rand_num = random_starter[i];
        float x = (i % COLUMNS);

        Square new_square = getSquare(i, to_pixel(x), 0, rand_num);
        squares.push_back(new_square);
    }
}

bool is_inside_matrix(int x, int y)
{
    return (x >= 0 && x < COLUMNS) && (y >= 0 && y < ROWS);
}

std::vector<std::vector<int>> possible_directions(int column, int line)
{
    std::vector<std::vector<int>> directions = {
        {column, line + 1}, // top
        {column + 1, line}, // rigth
        {column, line - 1}, // bottom
        {column - 1, line}, // left
    };
    return directions;
}

bool emptyPosition(std::vector<Square> squares, int position)
{
    return position == -1 || (squares[position].index == -1);
}

bool direction_possible(std::vector<Square> &squares, int column, int line, int newSquareIndex) 
{

    if (emptyPosition(squares, newSquareIndex))
    {
        return true;
    }

    int newCol = get_column(squares[newSquareIndex].x);
    int newLine = get_line(squares[newSquareIndex].y);

    std::vector<std::vector<int>> directions = possible_directions(column, line);

    bool found = false;
    for (const auto &direction : directions)
    {
        if (direction[0] == newCol && direction[1] == newLine)
        {
            found = true;
            break;
        }
    }

    return found;
}
bool is_possible_direction(std::vector<Square> &squares, int oldSquareIndex, int newSquareIndex) //Dire si cette direction est possible pour la position initial
{

    Square square = squares[oldSquareIndex];

    int column = get_column(get_x(square.index));
    int line = get_line(get_y(square.index));
    return direction_possible(squares, column, line, newSquareIndex);
}

bool current_is_possile_direction(std::vector<Square> &squares, int oldSquareIndex, int newSquareIndex) //Dire si cette direction est possible avec les x et y courant 
{
    Square square = squares[oldSquareIndex];

    int column = get_column(square.x);
    int line = get_line(square.y);
    return direction_possible(squares, column, line, newSquareIndex);
}

int findSqaureWithIndex(std::vector<Square> squares, int indexInMatrix)
{
    auto it = std::find_if(squares.begin(), squares.end(),
                           [indexInMatrix](const Square &element)
                           {
                               return element.index == indexInMatrix;
                           });

    if (it != squares.end())
    {
        // Calculate the index by subtracting the iterators
        return std::distance(squares.begin(), it);
    }
    else
    {
        // Return a special value or handle the case when the element is not found
        return -1;
    }
}

int newSquarePosition(std::vector<Square> &squares, int newCol, int newLine)
{
    if (!is_inside_matrix(newCol, newLine))
    {
        return -1;
    }

    return findSqaureWithIndex(squares, get_index(newCol, newLine));
}

bool canMoveSquare(std::vector<Square> &squares, int squareIndex, int newPosition)
{

    return emptyPosition(squares, newPosition) || (squareIndex == newPosition); //Soit il revient sur place soit vers une vers case vide
}

bool canBeMerged(std::vector<Square> &squares, int squareIndex, int newPosition)
{
    if (emptyPosition(squares, newPosition))
    {
        cout << "Empty position can't merge \n";
        return false;
    }

    return (squareIndex != newPosition) &&    //les conditions pour qu'il fusionne 
           (squares[newPosition].index != -1) &&
           (squares[squareIndex].index != -1) &&
           (squares[squareIndex].number == squares[newPosition].number);
}

bool is_merging_state(std::vector<Square> &squares, int oldSquareIndex, int currentSquareIndex, bool isMerging)
{
    int oldPosition = squares[oldSquareIndex].index;
    return (isMerging && (oldSquareIndex != currentSquareIndex && squares[oldSquareIndex].number == squares[currentSquareIndex].number)); //meme nombre et index différent
}

void mergeTwoSquares(std::vector<Square> &squares, int squareInIndex, int existentSquareIndex)
{
    Square existentSquare = squares[existentSquareIndex];

    // squares[squareInIndex].index = existentSquare.index;
    // squares[squareInIndex].x = existentSquare.x;
    // squares[squareInIndex].y = existentSquare.y;
    cout << "Merge " << squares[squareInIndex].index << " index with " << existentSquare.index << " index \n";
    cout << "Remove square with number " << existentSquare.number << " at position " << existentSquare.index << " \n";

    // squares.erase(squares.begin() + existentSquareIndex); // remove the merged square
    int newNumber = squares[squareInIndex].number + 1;
    squares[squareInIndex].number = newNumber;
    squares[squareInIndex].color = beautifulColors[newNumber % beautifulColors.size()]; //Pour que le reste soit toujours entre 0 et max des couleurs
    squares[squareInIndex].isMoving = false;
    squares[existentSquareIndex].index = -1;
}

/*
 Déplacer un carré spécifique dans le vecteur squares vers de nouvelles coordonnées
*/
void moveSquare(std::vector<Square> &squares, int index, float newX, float newY)
{
    if (index >= 0 && index < squares.size())
    {
        cout << "Mouse moved the square with number " << squares[index].number << " to (x: " << newX << " , y: " << newY << " )\n";
        squares[index].x = newX;
        squares[index].y = newY;
        squares[index].isMoving = true;
    }
    else
    {
        // Handle the case where 'index' is out of range
        // For instance, print an error message or throw an exception
    }
}

/*
Déterminer l'indice du carré dans une grille (ou une structure similaire)
sur laquelle le clic de souris a eu lieu, en se basant sur les coordonnées de la souris
*/
int detectSquare(float mouseX, float mouseY)
{
    int column = get_mouse_column(mouseX);
    int row = get_mouse_line(mouseY);

    return get_index(column, row);
}

/*
 Trouver la première ligne contenant un carré dans une colonne spécifique,
 déterminée par les coordonnées x et y du carré actuel
*/

int firstRowWithSquareInColumn(std::vector<Square> &squares, Square currentSquare, int maxRow)
{

    int oldX = get_column(currentSquare.x);
    int oldY = get_line(currentSquare.y);

    int lastY = 0;
    bool found = false;

    for (int row = 0; row < oldY; row++)
    {

        int square_index = findSqaureWithIndex(squares, get_index(oldX, row));

        if (square_index == -1 && lastY > row)
        {
            lastY = row;
            found = true;
        }
        else if (lastY != -1 && square_index != -1)
        {
            if (squares[square_index].index != currentSquare.index)
            {
                found = true;
                lastY = row;
            }
        }
    }

    return found ? lastY : -1;
}

void updateSquare(std::vector<Square> &squares, int squareIndex, int newLine, int newColumn)
{
    int newIndex = get_index(newColumn, newLine);

    squares[squareIndex].index = newIndex;
    squares[squareIndex].y = to_pixel(newLine);
    squares[squareIndex].isMoving = false;
}

/*
 Met à jour les indices des carrés dans un vecteur squares après qu'un carré a été déplacé
*/
Square updateSquareIndex(std::vector<Square> &squares, int squareIndex, int oldSquareY)
{

    if (squareIndex == -1)
    {
        std::cout << " Not updating anything \n";
        return {};
    }

    Square oldSquare = squares[squareIndex];
    // cout << "Updating square position with max row " << oldSquareY << '\n';
    int oldRow = get_line(oldSquare.y);
    int row = firstRowWithSquareInColumn(squares, oldSquare, oldRow) + 1;
    int newColumn = get_column(oldSquare.x); // already updated by the mouse mouvement

    if (row != -1 && row <= oldRow)
    {
        // std::cout << "Gravity update of the square with number " << square.number << "\n";
        updateSquare(squares, squareIndex, row, newColumn);
    }

    return oldSquare;
}

void printMatrix(const std::vector<Square> &squares)
{
    for (int i = (ROWS * COLUMNS) - 1; i >= 0; i--)
    {
        int index = findSqaureWithIndex(squares, i);
        if (index != -1)
        {
            Square s = squares[index];
            std::cout << ' ' << s.number << ' ';
        }
        else
        {
            std::cout << ' ' << '.' << ' ';
        }

        if (i % (ROWS - 1) == 0)
        {
            std::cout << std::endl;
        }
    }
}