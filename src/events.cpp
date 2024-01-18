
#include <iostream>
#include "controle.cpp"

bool canOnlyBeMergin = false; ////Bordure blanche  
bool canMove = false; //Bordure rouge 


/*

Fonction qui permet de détécter quel square qu'on a sellectionner et 
elle permet de dire si on peut le faire bouger ou fusionner ou rien

*/
int clickOnSquare(std::vector<Square> &squares, int mouseX, int mouseY, bool &isDragging, bool &isMerging, int &oldSquareY)
{

    canOnlyBeMergin = false;
    isDragging = false;
    int squareIndex = detectSquare(mouseX, mouseY); // to know what to move
    int index = findSqaureWithIndex(squares, squareIndex);
    if (index != -1)
    {
        std::cout << "Detected square with number " << squares[index].number << " in position " << squares[index].index << '\n';
        int column = get_mouse_column(mouseX);
        int line = get_mouse_line(mouseY);

        std::vector<std::vector<int>> directions = possible_directions(column, line);

        for (int i = 0; i < directions.size(); i++)
        {
            int c = directions[i][0];
            int l = directions[i][1];
            int newPosition = newSquarePosition(squares, c, l);
            if (is_inside_matrix(c, l))
            {

                cout << "test movement to line " << l << " and column " << c << " with number " << squares[newPosition].number << " and index " << squares[newPosition].index << "\n";
                if (canMoveSquare(squares, index, newPosition))
                {
                    cout << "Can move to line " << l << " and column " << c << " \n";
                    canMove = true;
                    break;
                }
                else if (canBeMerged(squares, index, newPosition))
                {
                    cout << "can only move to be merged to line " << l << " and column " << c << " \n";
                    canOnlyBeMergin = true;
                }
            }
        }
        if (canMove || canOnlyBeMergin)
        {
            canOnlyBeMergin = !canMove;
            isDragging = true;
            squareIndex = index;
            squares[index].isMoving = true;
            oldSquareY = squares[index].y;
        }
        else
        {
            cout << "Square can't be moved \n";
        }
    }
    else
    {
        squareIndex = -1;
        std::cout << "No square detected \n";
    }

    return squareIndex;
}


/*
Fusionne les squares si possibles et met à jour les index des sqaures
*/
void releaseSquare(std::vector<Square> &squares, int mouseX, int mouseY, int squareIndex, int oldSquareY, bool &isDragging, bool &isMerging)
{
    int column = get_mouse_column(mouseX);
    int line = get_mouse_line(mouseY);
    int newPosition = newSquarePosition(squares, column, line);
    // cout << "New position going " << newPosition << '\n';

    int is_merging = is_merging_state(squares, squareIndex, newPosition, isMerging);
    // cout << "Merge? " << squareIndex << " with " << newPosition << "\n";
    if (newPosition != -1 && squareIndex != newPosition && is_merging) //dans le cas ou on peut fusionner deux squares
    {
        cout << "Merged " << squareIndex << " with " << newPosition << "\n";
        mergeTwoSquares(squares, squareIndex, newPosition);
        playMergeSquareMusic();
        isMerging = false;
        canOnlyBeMergin = false;
    }

    if (squareIndex != -1) //pour metre à jour avec le nouveau index de la matrice
    {
        updateSquareIndex(squares, squareIndex, oldSquareY);
    }
    isDragging = false;
    canMove = false;
    glutPostRedisplay();
}
/*
Fonction pour bouger le square dans toutes les positions possiblles 
*/
void dragSquare(std::vector<Square> &squares, int mouseX, int mouseY, int squareIndex, bool isDragging, bool &isMerging)
{

    if (!isDragging && !isMerging)  //pour revérifer une autre fois en cas ou 
    {
        cout << "Not dragging or merging state \n";
        return;
    }

    int newCol = get_mouse_column(mouseX);
    int newLine = get_mouse_line(mouseY);

    if (!is_inside_matrix(newCol, newLine))
    {
        cout << "cant' move out of the matrix \n"
             << '\n';
        return;
    }

    int newPosition = newSquarePosition(squares, newCol, newLine);

    cout << " new position " << newPosition << "\n";

    if (canOnlyBeMergin && isMerging) //dans le cas ou le square est déjà dans un autre square et veut bouger encore plus 
    {
        if (newPosition == squareIndex) //Le square peut juste revenir à sa postion initiale
        {
            isMerging = false;
            moveSquare(squares, squareIndex, to_pixel(newCol), to_pixel(newLine));
            glutPostRedisplay(); // Indiquer que la fenêtre doit être redessinée
        }
        else
        {
            cout << "Can only be merged or back to initial state";
        }
    }
    else //dans la plupart des cas 
    {
        if (canMoveSquare(squares, squareIndex, newPosition) && !canOnlyBeMergin) //cas ou le square peut bouger librement et dans le cas ou il pas fusionne pas uniquement 
        {
            isMerging = false;
            cout << "Normal move \n";
            moveSquare(squares, squareIndex, to_pixel(newCol), to_pixel(newLine));
            glutPostRedisplay(); // Indiquer que la fenêtre doit être redessinée
            return;
        }
        else if (canBeMerged(squares, squareIndex, newPosition) && !isMerging)
        {
            cout << "Merge move\n";
            if (canOnlyBeMergin && !is_possible_direction(squares, squareIndex, newPosition)) //Pour pas fusionner deux square par les cotés 
            {
                return;
            }

            if (canMove && !current_is_possile_direction(squares, squareIndex, newPosition)) //Pour empecher de sauter des squares pour fusionner un autre 
            {
                return;
            }

            isMerging = true;
            // canOnlyBeMergin = true;
            moveSquare(squares, squareIndex, to_pixel(newCol), to_pixel(newLine));
            glutPostRedisplay(); // Indiquer que la fenêtre doit être redessinée
            return;
        }
    }

    cout << "Can't be dragged\n";
}

/*
 re-intiliaze the global variable of the game
*/

void restart_game(std::vector<Square> &squares, float &timerBarWidth, int &maxDisplayedLine)
{
    timerBarWidth = 0;
    maxDisplayedLine = 0;
    initSquares(squares, COLUMNS * STARTER_LINES);
}