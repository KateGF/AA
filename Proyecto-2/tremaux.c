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

                printf("Up to %d \n", valueUp);
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
            // printf( "___%d_______Visited ____%d____\n" , previousPositionX,previousPositionY);
            currentPositionX = newPositionX;
            currentPositionY = newPositionY;
        }
        moved = false;
    }
}