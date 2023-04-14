#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>


/*******************************Look Up Table Matrix Functions*******************************/
/* Look Up Table
    The creation of this look up table utilices the following reference:
Jones, N. (2010, January 11). A tutorial on lookup tables in C «  Stack Overflow. https://embeddedgurus.com/stack-overflow/2010/01/a-tutorial-on-lookup-tables-in-c/
*/

/*******************************Principal Algorithm*******************************/
/*Rules
// -1 ; Frontier Cells
// 0 ; Spanning Tree Cells
// + 1 ; Right
// + 2 ; Below
// + 4 ; Left
// + 8 ; Above
RULES*/

/*Situations
0 ; All borders
1 ; No right border
2 ; No below border
3 ; No right, below border
4 ; No left border
5 ; No left, right border
6 ; No left, below border
7 ; No right, below, left border
8 ; No upper border
9 ; No upper, right border
10 ; No upper, below border
11 ; No above, below, right border
12 ; No left, upper border
13 ; No left, upper, right border
14 ; No above, left, below
15 ; No borders
Situations*/

#define topBorder    1
#define rightBorder  2
#define bottomBorder 4
#define leftBorder   8

const int borders[] = {
    topBorder | rightBorder | bottomBorder | leftBorder, // 15
    topBorder | bottomBorder | leftBorder, // 13
    topBorder | rightBorder | leftBorder,  // 11
    topBorder | leftBorder, // 9
    topBorder | rightBorder | bottomBorder, // 7
    topBorder | bottomBorder, // 5
    topBorder | rightBorder, // 3
    topBorder, // 1
    rightBorder | bottomBorder | leftBorder, // 14
    bottomBorder | leftBorder, // 12
    rightBorder | leftBorder, // 10
    leftBorder, // 8
    bottomBorder | rightBorder, // 6
    bottomBorder, // 4
    rightBorder, // 2
    0
};

/*******************************General Matrix Functions*******************************/

void fillMatrix(int **matrix, int rows, int columns, int value) {
    /*
    Subroutine that fills the array with equal values.
    Inputs and constraints:
        -matrix: Pointer to the matrix made up of integers to be filled with value.
        -rows: Non-negative integer indicating the number of rows in the matrix.
        -columns: Non-negative integer indicating the number of columns in the matrix.
        -value: The value with which all the fields of the matrix will be filled.
        Outputs:
    -The matrix with all its fields with the value received by parameter.
    */
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            matrix[row][column] = value;
        }
    }
}

int **createMatrix(int rows, int columns) { //

    /*
    Subroutine that is in charge of creating an array from its number of rows and columns.
    Inputs and constraints:
        -rows: Non-negative integer indicating the number of rows in the matrix.
        -columns: Non-negative integer indicating the number of columns in the matrix.
    Outputs:
    -The correctly created matrix.
    References:
        -Portfolio Courses. (2022a, September 2). Return A Dynamically Allocated 2D Array From A Function | C Programming Tutorial [Video]. YouTube. https://www.youtube.com/watch?v=22wkCgsPZSU
    */

    int **matrix;

    matrix = malloc(sizeof(int *) * rows); // Creates the rows

    for (int row = 0; row < rows; row++) { // Creates de columns
        matrix[row] = malloc(sizeof(int *) * columns);
    }
    fillMatrix(matrix, rows, columns, 0);

    return matrix;
}

void freeMatrix(int **matrix, int rows) {
   /*
    Subroutine that returns the memory occupied by a matrix by iterating through its rows.
    Inputs and constraints:
        -matrix: Pointer to the matrix that will free its memory.
        -rows: Non-negative integer indicating the number of rows in the matrix.
    Outputs:
        -Memory occupied by the correctly freed matrix.
    */
    for (int row = 0; row < rows; row++) {
        free(matrix[row]);
    }
    free(matrix);
}

int initialCellStarterValue = 16;


void printLaberynth(int **matrix, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        printf("[ ");
        for (int z = 0; z < columns; z++) {
            printf("%d, ", matrix[i][z]);
        }
        printf("]\n");
    }

}
void adjacentCellsIntoFrotierCells(int **matrix, int rows, int columns, int positionX, int positionY, int *frontierCellsXPosition, int *frontierCellsYPosition, int *frotierCellsArraySize) {
    /*
    Subroutine that converts the adjacent cells of a given position in a matrix into frontier cells.
    Inputs and constraints:
        - matrix: A pointer to a two-dimensional array of integers representing the matrix.
        - rows: The number of rows in the matrix.
        - columns: The number of columns in the matrix.
        - positionX: The X-coordinate of the position in the matrix whose adjacent cells to convert. coor x spanning
        - positionY: The Y-coordinate of the position in the matrix whose adjacent cells to convert. coor y del spanning
        - frontierCellsXPosition: A pointer to an array of integers to store the X-coordinates of the resulting frontier cells. guardo coord x en array
        - frontierCellsYPosition: A pointer to an array of integers to store the Y-coordinates of the resulting frontier cells.  guardo coord y en array
        - frotierCellsArraySize: A pointer to an integer to store the size of the resulting frontier cells array.
    Outputs:
        -The adjacent cell of the given position in the matrix into a frontier cell.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
 */    

    if (positionX - 1 >= 0 && matrix[positionX - 1][positionY] == 0) { // Up
        matrix[positionX - 1][positionY] = -1;
        frontierCellsXPosition[*frotierCellsArraySize] = positionX - 1;
        frontierCellsYPosition[*frotierCellsArraySize] = positionY;
        *frotierCellsArraySize = *frotierCellsArraySize + 1;
    }

    if (positionX + 1 < rows && matrix[positionX + 1][positionY] == 0) { // Down
        matrix[positionX + 1][positionY] = -1;
        frontierCellsXPosition[*frotierCellsArraySize] = positionX + 1;
        frontierCellsYPosition[*frotierCellsArraySize] = positionY;
        *frotierCellsArraySize = *frotierCellsArraySize + 1;
    }

    if (positionY - 1 >= 0 && matrix[positionX][positionY - 1] == 0) { // Left
        matrix[positionX][positionY - 1] = -1;
        frontierCellsXPosition[*frotierCellsArraySize] = positionX;
        frontierCellsYPosition[*frotierCellsArraySize] = positionY - 1;
        *frotierCellsArraySize = *frotierCellsArraySize + 1;
    }

    if (positionY + 1 < columns && matrix[positionX][positionY + 1] == 0) { // Right
        matrix[positionX][positionY + 1] = -1;
        frontierCellsXPosition[*frotierCellsArraySize] = positionX;
        frontierCellsYPosition[*frotierCellsArraySize] = positionY + 1;
        *frotierCellsArraySize = *frotierCellsArraySize + 1;
    }
}

void searchRandomFrontierCell(int *positionX, int *positionY, int *frontierCellsXPosition, int *frontierCellsYPosition, int *frontierCellsArraySize) { // Searchs a random Frontier Cell
    /*
    Subroutine that is in charge of searching for a random border cell.
    Inputs and constraints:
        -positionX: Pointer to the X position of the selected cell.
        -positionY: Pointer to the Y position of the selected cell.
        -frontierCellsXPosition: Pointer to the array of X positions of the border cells.
        -frontierCellsYPosition: Pointer to the array of Y positions of the border cells.
        -positionY: Pointer to the Y position of the selected cell.
        -frontierCellsArraySize: Pointer to the current size of the border cells array.
    Outputs:
        -The random frontier cell already selected.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */
    int randomPosition = rand() % (*frontierCellsArraySize);
    *positionX = frontierCellsXPosition[randomPosition];
    *positionY = frontierCellsYPosition[randomPosition];
    *frontierCellsArraySize = *frontierCellsArraySize - 1; // update size del array
    // recorta el tamano del array
    for (int i = randomPosition; i < *frontierCellsArraySize; i++) {
        frontierCellsXPosition[i] = frontierCellsXPosition[i + 1];
        frontierCellsYPosition[i] = frontierCellsYPosition[i + 1];
    }
}
 
void searchRandomSpanningTreeCell(int **matrix, int rows, int columns, int positionX, int positionY, int *randomSpanningTreeCellX, int *randomSpanningTreeCellY) { // Search a random Spanning Tree Cell adjacent to a Frontier Cell
    /*
    Subroutine that searches for a random cell of the spanning tree adjacent to a boundary cell in a given array.
        -matrix: The matrix to search in.
        -rows: The number of rows in the matrix.
        -columns: The number of columns in the matrix.
        -positionX: The x-position of the boundary cell.
        -positionY: The y position of the border cell.
        -randomSpanningTreeCellX: pointer to a variable where the x position of the random cell of the spanning tree will be stored.
        -randomSpanningTreeCellY: A pointer to a variable where the y-position of the random Spanning Tree Cell will be stored.
    Outputs:
        -randomSpanningTreeCell adjacent to a boundary cell in a given matrix.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */

   // si no se ha salido del borde y no esta en spanning tree ya
    *randomSpanningTreeCellX = positionX;
    *randomSpanningTreeCellY = positionY;

    bool flag = true;                                      // -1 -1 -1 -1 -1
                                                           // -1  7

    while (flag) {    // 1,1
        switch (rand() % 4) {  // 0  1  mat[0][1]
        case 0: // si le resto uno a mi fila y no me he salido del borde de arriba
            if (positionX - 1 >= 0 && matrix[positionX - 1][positionY] > 0) { // Up
                *randomSpanningTreeCellX -= 1;
                flag = false;
            }
            break;
        case 1:// si es ultima fila y no me he salido
            if (positionX + 1 < rows && matrix[positionX + 1][positionY] > 0) {
                *randomSpanningTreeCellX += 1;
                flag = false;
            }
            break;
        case 2:
            if (positionY - 1 >= 0 && matrix[positionX][positionY - 1] > 0) {
                *randomSpanningTreeCellY -= 1;
                flag = false;
            }
            break;
        case 3:
            if (positionY + 1 < columns && matrix[positionX][positionY + 1] > 0) {
                *randomSpanningTreeCellY += 1;
                flag = false;
            }
            break;
        }
    }
}

void removeBarrierCell(int **matrix, int frontierX, int frontierY, int spanningTreeCellX, int spanningTreeCellY) { // Removes a barrier cell between two coordinates
    /*
    Subroutine that updates a matrix to remove a barrier cell between two coordinates. Checks the relative
    positions of the two cells and update the matrix accordingly.
    Inputs and constraints:
        -array: A pointer to a 2D array of integers representing the matrix.
        -frontierX: An integer representing the x-coordinate of the cell boundary.
        -frontierY: An integer representing the y-coordinate of the boundary cell.
        -spanningTreeCellX: An integer representing the x-coordinate of the spanning tree cell.
        -spanningTreeCellY: An integer representing the y-coordinate of the spanning tree cell.
    Outputs:
        -The matrix with the barrier cell removed between the two coordinates.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */
    if (frontierX != spanningTreeCellX) {
        if (frontierX > spanningTreeCellX) {
            matrix[frontierX][frontierY] = matrix[frontierX][frontierY] + 8; // suma 8 a la frontera
            matrix[spanningTreeCellX][spanningTreeCellY] = matrix[spanningTreeCellX][spanningTreeCellY] + 2; // suma dos al spanning del q pertence la frontera
        } else {
            matrix[frontierX][frontierY] = matrix[frontierX][frontierY] + 2;
            matrix[spanningTreeCellX][spanningTreeCellY] = matrix[spanningTreeCellX][spanningTreeCellY] + 8;
        }
    } else {
        if (frontierY > spanningTreeCellY) {
            matrix[frontierX][frontierY] = matrix[frontierX][frontierY] + 4;
            matrix[spanningTreeCellX][spanningTreeCellY] = matrix[spanningTreeCellX][spanningTreeCellY] + 1;
        } else {
            matrix[frontierX][frontierY] = matrix[frontierX][frontierY] + 1;
            matrix[spanningTreeCellX][spanningTreeCellY] = matrix[spanningTreeCellX][spanningTreeCellY] + 4;
        }
    }
}
// choose random position y le pone valor de 16
void stepOne(int **matrix, int rows, int columns, int *positionX, int* positionY, int *frontierCellsXPosition, int *frontierCellsYPosition, int *frotierCellsArraySize) {
    /*
    Subroutine that executes the step one of the algorithm
    Inputs and constraints:
        -matrix: A pointer to a pointer of integers which represents a 2-dimensional array.
        -rows: The number of rows in the matrix.
        -columns: The number of columns in the matrix.
        -positionX: The x position of a randomly selected starting cell.
        -positionY: The y position of a randomly selected starting cell.
        -frontierCellsXPosition: Stores the x position of the frontier cell.
        -frontierCellsYPosition: Stores the y position of the frontier cell.
        -frotierCellsArraySize: A pointer to the number of frontier cells.
    Outputs:
        -The complete execution of the step one
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */

    int randomXPosition = rand() % rows;
    int randomYPosition = rand() % columns;

    matrix[randomXPosition][randomYPosition] = initialCellStarterValue;
    *positionX = randomXPosition;
    *positionY = randomYPosition;

    adjacentCellsIntoFrotierCells(matrix, rows, columns, randomXPosition, randomYPosition, frontierCellsXPosition, frontierCellsYPosition, frotierCellsArraySize);
}

void stepTwoAndThree(int **matrix, int rows, int columns, int *frontierCellsXPosition, int *frontierCellsYPosition, int *frotierCellsArraySize) { // Step Two and Three of the algorithm
    /*
    Subroutine that executes the steps two and three of the algorithm.
    Inputs and constraints:
        -matrix: A pointer to a pointer of integers which represents a 2-dimensional array.
        -rows: The number of rows in the matrix.
        -columns: The number of columns in the matrix.
        -frontierCellsXPosition: Stores the x position of the frontier cell.
        -frontierCellsYPosition: Stores the y position of the frontier cell.
        -frotierCellsArraySize: A pointer to the number of frontier cells.
    Outputs:
        -The complete execution of the steps two and three.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */
    int randomFrontierCellX, randomFrotierCellY;
    searchRandomFrontierCell(&randomFrontierCellX, &randomFrotierCellY, frontierCellsXPosition, frontierCellsYPosition, frotierCellsArraySize);
    int randomSpanningTreeCellX, randomSpanningTreeCellY;
     //printf("search Random FrontierCell\n");
     //printLaberynth(matrix, rows, columns);
     //    printf("\n");

    searchRandomSpanningTreeCell(matrix, rows, columns, randomFrontierCellX, randomFrotierCellY, &randomSpanningTreeCellX, &randomSpanningTreeCellY);
    matrix[randomFrontierCellX][randomFrotierCellY] = 0;
     //printf("search Random Spanning TreeCell\n");
     //printLaberynth(matrix, rows, columns);
        // printf("\n");

    removeBarrierCell(matrix, randomFrontierCellX, randomFrotierCellY, randomSpanningTreeCellX, randomSpanningTreeCellY);
 //printf("Remove Barrier Cell\n");
    // printLaberynth(matrix, rows, columns);
      //   printf("\n");

    adjacentCellsIntoFrotierCells(matrix, rows, columns, randomFrontierCellX, randomFrotierCellY, frontierCellsXPosition, frontierCellsYPosition, frotierCellsArraySize);
   // printf("Adjacent\n");
     //printLaberynth(matrix, rows, columns);
      //   printf("\n");
            
    //printf("Otra vuelta ******************************************** \n\n");
}

bool stepFour(int frotierCellsArraySize) {
    /*
    Subroutine that executes the step four of the algorithm.
    Inputs and constraints:
        -frotierCellsArraySize: The number of frontier cells.
    Outputs:
        -The complete execution of the step four.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */
    return frotierCellsArraySize > 0;
}

void stepFive(int **matrix, int rows, int columns, int initialCellXPosition, int initialCellYPosition) {
    /*
    Subroutine that executes the step five of the algorithm.
    Inputs and constraints:
        -matrix: A pointer to a pointer of integers which represents a 2-dimensional array.
        -rows: The number of rows in the matrix.
        -columns: The number of columns in the matrix.
        -initialCellXPosition: Stores the x position of the initial cell.
        -initialCellYPosition: Stores the y position of the initial cell.
    Outputs:
        -The complete execution of the step five.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */
    matrix[0][0] = matrix[0][0] + 8;
    matrix[rows - 1][columns - 1] = matrix[rows - 1][columns - 1] + 2;
    matrix[initialCellXPosition][initialCellYPosition] = matrix[initialCellXPosition][initialCellYPosition] - initialCellStarterValue;
}


/*******************************Laberynth Main Functions*******************************/

int** createLaberynth(int rows, int columns) {
    /*
    Subroutine that is in charge of creating the laberynth and storing it in a matrix.
    Inputs and constraints:
        -rows: The number of rows in the matrix.
        -columns: The number of columns in the matrix.
    Outputs:
        -The matrix containing the values of the laberynth.
    References:
	-Matuszek, D. (n.d.). How to build a maze. Retrieved from https://www-fourier.ujf-grenoble.fr/~faure/enseignement/projets_simulation/labyrinthe/construct_a_maze.pdf
    */

    int **matrix = createMatrix(rows, columns);
    srand(time(NULL));

    int initialCellXPosition;
    int initialCellYPosition;

    int frontierCellsArrayMaxSize = 3 * (rows + columns);
    int frontierCellsXPosition[frontierCellsArrayMaxSize];
    int frontierCellsYPosition[frontierCellsArrayMaxSize];
    int frontierCellsArraySize = 0;


    stepOne(matrix, rows, columns, &initialCellXPosition, &initialCellYPosition, frontierCellsXPosition, frontierCellsYPosition, &frontierCellsArraySize);
    
    //printLaberynth(matrix, rows, columns);
       //  printf("\n");
    while (stepFour(frontierCellsArraySize))
        stepTwoAndThree(matrix, rows, columns, frontierCellsXPosition, frontierCellsYPosition, &frontierCellsArraySize);
    stepFive(matrix, rows, columns, initialCellXPosition, initialCellYPosition);

    return matrix;
}



/*******************************Project - 2*******************************/

/*

comprueban si la dirección correspondiente está bloqueada o no por una pared, 
y devuelve verdadero si la dirección no está bloqueada.

*/

// recibe un valor como 14 busca en la tabla y hace un AND si me da cero, devuelve true y no tiene ese borde
bool canGoUp(int positionValue) { 
   
    bool res = !(borders[positionValue] & topBorder);

    return res;
}

bool canGoDown(int positionValue) {
    return !(borders[positionValue] & bottomBorder);
}

bool canGoRight(int positionValue) {
    return !(borders[positionValue] & rightBorder);
}

bool canGoLeft(int positionValue) {
    return !(borders[positionValue] & leftBorder);
}

/*******************************Random Mouse*******************************/ 
void newRandomPosition(int **matrix, int rows, int columns, int currentPositionX, int currentPositionY, int *newPositionX, int *newPositionY) {
    bool flag = true;
    while (flag) {
        switch (rand() % 4) {
        case 0: // el % 16 es para asegurarse que no se salga del rango de indices de la tabla
            if (currentPositionX - 1 >= 0 && canGoUp(matrix[currentPositionX][currentPositionY] % 16)) { // Up
                *newPositionX -= 1;
                 // printf("newPositionX en newRandomPosition: %d\n", newPositionX);
        // printf("currentPositionY antes newRandom: %d\n", currentPositionY);
                flag = false;
            }
            break;
        case 1:
            if (currentPositionX + 1 < rows && canGoDown(matrix[currentPositionX ][currentPositionY] % 16)) {
                *newPositionX += 1;
                //printf("newPositionX en newRandomPosition: %d\n", newPositionX);
                flag = false;
            }
            break;
        case 2:
            if (currentPositionY - 1 >= 0 && canGoLeft(matrix[currentPositionX][currentPositionY] % 16)) {
                *newPositionY -= 1;
                //printf("newPositionY en newRandomPosition: %d\n", newPositionY);
                flag = false;
            }
            break;
        case 3:
            if (currentPositionY + 1 < columns && canGoRight(matrix[currentPositionX][currentPositionY] % 16)) {
                *newPositionY += 1;
                //printf("newPositionY en newRandomPosition: %d\n", newPositionY);
                flag = false;
            }
            break;
        }
    }
}

void randomMouse(int **matrix, int rows, int columns) {
    // Start and End values
    int laberynthEntranceX = 0; 
    int laberynthEntranceY = 0;
    int laberynthExitX = rows - 1;
    int laberynthExitY = columns - 1;

    // Algorithm
    int currentPositionX = laberynthEntranceX;
    int currentPositionY = laberynthEntranceY;
    int newPositionX = currentPositionX;
    int newPositionY = currentPositionY;

    // Assign First Position as visited
    matrix[currentPositionX][currentPositionY] = matrix[currentPositionX][currentPositionY] + 16;

    int totalCycles = 0;
    while(currentPositionX != laberynthExitX || currentPositionY != laberynthExitY) {
        // Select a random new Position
       // printf("currentPositionX antes newRandom: %d\n", currentPositionX);
        // printf("currentPositionY antes newRandom: %d\n", currentPositionY);
        //   printf("-----------------------------------------\n");
        newRandomPosition(matrix, rows, columns, currentPositionX, currentPositionY, &newPositionX, &newPositionY);

        // Set the values of the spot
        if (matrix[newPositionX][newPositionY] > 15) {
            matrix[currentPositionX][currentPositionY] = matrix[currentPositionX][currentPositionY] - 16;
        } else {
            matrix[newPositionX][newPositionY] = matrix[newPositionX][newPositionY] + 16;
        }

        //Assign values
        currentPositionX = newPositionX;
        currentPositionY = newPositionY;
        totalCycles++;
    }
    printf("Total Cycles: %d\n", totalCycles);
}

/*******************************Right Hand Rule*******************************/
void rightHandRule(int **matrix, int rows, int columns){}

/*******************************Left Hand Rule*******************************/
void leftHandRule(int **matrix, int rows, int columns) {}

void PledgeAlgorithm(int **matrix, int rows, int columns) {}


void tremaux(int **matrix, int rows, int columns)
{
    // Start and End values
    int laberynthEntranceX = 0;
    int laberynthEntranceY = 0;
    int laberynthExitX = rows - 1;
    int laberynthExitY = columns - 1;

    // Algorithm
    int currentPositionX = laberynthEntranceX;
    int currentPositionY = laberynthEntranceY;
    int newPositionX = currentPositionX;
    int newPositionY = currentPositionY;
    int previousPositionX = currentPositionX;
    int previousPositionY = currentPositionY;

    bool moved = false;
    bool visitedTwice = false;

    while ((currentPositionX != laberynthExitX) || (currentPositionY != laberynthExitY))
    {
        int value = matrix[currentPositionX][currentPositionY];
        bool canGoUpp = canGoUp(value % 16);
        bool canGoDownn = canGoDown(value % 16);
        bool canGoLeftt = canGoLeft(value % 16);
        bool canGoRightt = canGoRight(value % 16);

        // Check if it can go up
        if (currentPositionX - 1 > 0 && canGoUpp)
        {
            newPositionX = currentPositionX - 1;
            int valueUp = matrix[newPositionX][currentPositionY];
            if (valueUp < 16)
            {

                //printf("Up to %d \n", valueUp);
                moved = true;
                previousPositionX = currentPositionX;
                previousPositionY = currentPositionY;
                currentPositionX = newPositionX;
                visitedTwice = false;
            }
            else
            {
                moved = false;
                visitedTwice = true;
            }
        }

        // Check if it can go down
        if (!moved && currentPositionX + 1 < rows && canGoDownn)
        {
            newPositionX = currentPositionX + 1;
            int valueDown = matrix[newPositionX][currentPositionY];
            if (valueDown < 16)
            {

                printf("Down to %d \n", valueDown);
                moved = true;
                visitedTwice = false;
                previousPositionX = currentPositionX;
                previousPositionY = currentPositionY;
                currentPositionX = newPositionX;
            }
            else
            {
                visitedTwice = true;
                moved = false;
            }
        }

        // Check if it can go left
        if (!moved && currentPositionY - 1 > 0 && canGoLeftt)
        {
            newPositionY = currentPositionY - 1;
            int valueLeft = matrix[currentPositionX][newPositionY];
            if (valueLeft < 16)
            {
                currentPositionY = newPositionY;
                printf("Left to %d \n", valueLeft);
                moved = true;
                visitedTwice = false;
            }
            else
            {
                visitedTwice = true;
            }
        }

        // Check if it can go right
        if (!moved && currentPositionY + 1 < columns)
        {
            newPositionY = currentPositionY + 1;
            int valueRight = matrix[currentPositionX][newPositionY];
            if (valueRight < 16)
            {

               printf("Right to %d \n", valueRight);
                moved = true;
                previousPositionX = currentPositionX;
                previousPositionY = currentPositionY;
                currentPositionY = newPositionY;
                visitedTwice = false;
            }
            else
            {
                visitedTwice = true;
            }
        }

        if (moved == true)
        {

            matrix[previousPositionX][previousPositionY] += 16;
            printf("__________________\n");
        }
        else
        {
             printf( "___%d_______Visited ____%d____\n" , previousPositionX,previousPositionY);
            currentPositionX = newPositionX;
            currentPositionY = newPositionY;
        }
        moved = false;
    }
}
//void TremauxAlgorithm(int **matrix, int rows, int columns) {}


void FattahAlgorithm(int **matrix, int rows, int columns) {}

/*******************************Main Program*******************************/

int main() {
    int rows = 5;
    int columns = 5;
    int **matrix = createLaberynth(rows, columns);

    printLaberynth(matrix, rows, columns);

    printf("- - - - - - - - - - - - - - - - - - - - - \n");

     //randomMouse(matrix, rows, columns);
      //printf("Random Mouse \n");
    //printLaberynth(matrix, rows, columns);
   printf("Solve Maze \n");
   tremaux(matrix, rows, columns);

   printf("- - - - - - - - - - - - - - - - - - - - - \n");
   printLaberynth(matrix, rows, columns);

    return 0;
}
