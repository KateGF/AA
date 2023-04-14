#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <stack.h>

// Definir las constantes para las paredes de cada celda
#define topBorder    1
#define rightBorder  2
#define bottomBorder 4
#define leftBorder   8

// Definir una tabla que indique las paredes de cada celda
const int borders[] = {
    topBorder | rightBorder | bottomBorder | leftBorder,
    topBorder | bottomBorder | leftBorder,
    topBorder | rightBorder | leftBorder,
    topBorder | leftBorder,
    topBorder | rightBorder | bottomBorder,
    topBorder | bottomBorder,
    topBorder | rightBorder,
    topBorder,
    rightBorder | bottomBorder | leftBorder,
    bottomBorder | leftBorder,
    rightBorder | leftBorder,
    leftBorder,
    bottomBorder | rightBorder,
    bottomBorder,
    rightBorder,
    0
};

// Función para resolver un laberinto dado en forma de matriz de enteros
void solveMaze(int maze[], int numRows, int numCols, int startX, int startY, int endX, int endY) {
    // Crear una matriz para marcar las celdas visitadas
    bool** visited = (bool**) malloc(numRows * sizeof(bool*));
    for (int i = 0; i < numRows; i++) {
        visited[i] = (bool*) calloc(numCols, sizeof(bool));
    }
    
    // Crear una pila para almacenar el camino
    std::stack<std::pair<int, int>> path;
    
    // Añadir la celda de inicio al camino y marcarla como visitada
    path.push(make_pair(startX, startY));
    visited[startX][startY] = true;
    
    // Iniciar el algoritmo de Tremaux
    int steps = 0;
    while (!path.empty()) {
        steps++;
        int x = path.top().first;
        int y = path.top().second;
        
        // Comprobar si se ha llegado a la celda de fin
        if (x == endX && y == endY) {
            break;
        }
        
        // Buscar una celda vecina que no haya sido visitada
        int prevX = x;
        int prevY = y;
        bool found = false;
        for (int i = 0; i < 4; i++) {
            int newX = x;
            int newY = y;
            switch (i) {
                case 0: newX--; break; // Arriba
                case 1: newY++; break; // Derecha
                case 2: newX++; break; // Abajo
                case 3: newY--; break; // Izquierda
            }
            // Comprobar si la celda vecina existe y no ha sido visitada
            if (newX >= 0 && newX < numRows && newY >= 0 && newY < numCols &&
                !visited[newX][newY] && !(maze[x*numCols+y] & borders[i])) {
                path.push(make_pair(newX, newY));
                visited[newX][newY] = true;
                found = true;
                break;
            }
        }
        
        if (!found) {
            // No se ha encontrado una celda vecina sin visitar, retroceder en el camino
            do {
                steps--;
                visited[x][y] = steps;
                path.pop();
                if (path.empty()) {
                    // Se ha retrocedido hasta el inicio del camino y no hay más celdas por visitar
                    printf("No se puede llegar al final del laberinto\n");
                    for (int i = 0; i < numRows; i++) {
                        free(visited[i]);
                    }
                    free(visited);
                    return;
                }
                x = path.top().first;
                y = path.top().second;
                // Comprobar si la celda actual tiene alguna celda vecina sin visitar
                found = false;
                for (int i = 0; i < 4; i++) {
                    int newX = x;
                    int newY = y;
                    switch (i) {
                        case 0: newX--; break; // Arriba
                        case 1: newY++; break; // Derecha
                        case 2: newX++; break; // Abajo
                        case 3: newY--; break; // Izquierda
                    }
                    // Comprobar si la celda vecina existe y no ha sido visitada
                    if (newX >= 0 && newX < numRows && newY >= 0 && newY < numCols &&
                        !visited[newX][newY] && !(maze[x*numCols+y] & borders[i])) {
                        found = true;
                        break;
                    }
                }
            } while (!found);
        }
    }
    
    // Imprimir la matriz de celdas visitadas
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            printf("%c ", visited[i][j] ? '.' : '#');
        }
        printf("\n");
    }
    
    // Liberar la memoria utilizada
    for (int i = 0; i < numRows; i++) {
        free(visited[i]);
    }
    free(visited);
}









/*#include <stdio.h>

#define topBorder    1
#define rightBorder  2
#define bottomBorder 4
#define leftBorder   8

const int borders[] = {
    topBorder | rightBorder | bottomBorder | leftBorder,
    topBorder | bottomBorder | leftBorder,
    topBorder | rightBorder | leftBorder,
    topBorder | leftBorder,
    topBorder | rightBorder | bottomBorder,
    topBorder | bottomBorder,
    topBorder | rightBorder,
    topBorder,
    rightBorder | bottomBorder | leftBorder,
    bottomBorder | leftBorder,
    rightBorder | leftBorder,
    leftBorder,
    bottomBorder | rightBorder,
    bottomBorder,
    rightBorder,
    0
};

void Tremaux(int matrix[][8], int rows, int cols, int x, int y, int prev_x, int prev_y, int visited) {
    int i, dir;
    
    // Si alcanzamos el borde de la matriz, detenemos la búsqueda
    if (x < 0 || x >= rows || y < 0 || y >= cols) {
        return;
    }
    
    // Si ya visitamos esta celda, detenemos la búsqueda
    if (visited & matrix[x][y]) {
        return;
    }
    
    // Marcamos la celda como visitada
    visited |= matrix[x][y];
    
    // Si llegamos a la meta, detenemos la búsqueda
    if (matrix[x][y] > 16) {
        return;
    }
    
    // Si esta celda no es un camino, detenemos la búsqueda
    if (matrix[x][y] == borders[15]) {
        return;
    }
    
    // Recorremos todas las direcciones posibles
    for (i = 0; i < 4; i++) {
        dir = (i + 2) % 4;
        if (dir == 0 && (matrix[x][y] & topBorder) == 0) {
            Tremaux(matrix, rows, cols, x - 1, y, x, y, visited);
        }
        if (dir == 1 && (matrix[x][y] & rightBorder) == 0) {
            Tremaux(matrix, rows, cols, x, y + 1, x, y, visited);
        }
        if (dir == 2 && (matrix[x][y] & bottomBorder) == 0) {
            Tremaux(matrix, rows, cols, x + 1, y, x, y, visited);
        }
        if (dir == 3 && (matrix[x][y] & leftBorder) == 0) {
            Tremaux(matrix, rows, cols, x, y - 1, x, y, visited);
        }
    }
    
    // Si hemos llegado a una celda que no ha sido visitada previamente, la marcamos como parte del camino
    if (visited != matrix[x][y] && (
        (prev_x == x - 1 && prev_y == y && (matrix[x][y] & topBorder) == 0) ||
        (prev_x == x && prev_y == y + 1 && (matrix[x][y] & rightBorder) == 0) ||
        (prev_x == x + 1 && prev_y == y && (matrix[x][y] & bottomBorder) == 0) ||
        (prev_x == x && prev_y == y - 1 && (matrix[x][y] & leftBorder) == 0))) {
        
        matrix[x][y] += 16;
    }
}

int main() {
    int matrix[][8] = {
        {10, 2, 1, 6, 3, 4, 1, 6},
        {10, 9, 5, 13, 14, 2, 1, 14},
        {9, 7, 4, 3, 13, 15, 7, 14},
        {1, 15, 7, 13, 6, 10, 8, 8},
        {1, 14, 8, 3, 14, 11, 4, 2},
        {1, 14, 2, 8, 10, 11, 7, 14},
        {3, 15, 14, 1, 12, 10, 10, 10},
        {8, 8, 8, 1, 5, 12, 8, 10}
    };
    int rows = 8;
    int cols = 8;
    int i, j;

    // Imprimimos la matriz original
    printf("Matriz original:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Llamamos a la función Tremaux para encontrar el camino
    Tremaux(matrix, rows, cols, 0, 0, -1, -1, 0);

    // Imprimimos la matriz con el camino encontrado
    printf("\nMatriz con camino:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
*/