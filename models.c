#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <unistd.h>
#include <termios.h>

#include "models.h"

// START Global Variables
int directionRow = 1; // Initial direction of the ball along the rows
int directionCol = 1; // Initial direction of the ball along the columns

// END Global Variables

// ------------------START INITIALIZATION----------------------

// SetUp Allocation
void initializeLevelAllocation(Level *plevel, int levelNumber)
{
    plevel->pSnoopy = (Snoopy *)malloc(sizeof(Snoopy));
    plevel->pBall = (Ball *)malloc(sizeof(Ball));

    switch (levelNumber)
    {
    case 1:
        plevel->pWalls = (Wall *)malloc(sizeof(Wall) * LEVEL1_WALLS);
        plevel->pTraps = (Trap *)malloc(sizeof(Trap) * LEVEL1_TRAPS);
        break;
    }
}

// SetUp Params
void initializeParams(Level *plevel)
{
    plevel->score = 0;
    plevel->time = TIME;
    plevel->pSnoopy->lives = LIVES;
}

// SetUp Symbols
void initializeSymbols(Level *plevel)
{

    plevel->pSnoopy->symbol = 'P';
    plevel->pBall->symbol = 'o';

    for (int i = 0; i < 4; i++)
    {
        plevel->pBirds[i].symbol = '^';
    }

    plevel->pWalls->symbol = '#';
    plevel->pTraps->symbol = '*';
}

// SetUp Pos
void initializePos(Level *plevel)
{
    srand((unsigned int)time(NULL));
    // Place Snoopy in the center of the grid map
    plevel->pSnoopy->position.lig = DIMENSION / 2;
    plevel->pSnoopy->position.col = DIMENSION / 2;

    // Place birds in the corners of the grid map
    plevel->pBirds[0].position.lig = 1;
    plevel->pBirds[0].position.col = 1;

    plevel->pBirds[1].position.lig = 1;
    plevel->pBirds[1].position.col = DIMENSION - 2;

    plevel->pBirds[2].position.lig = DIMENSION - 2;
    plevel->pBirds[2].position.col = 1;

    plevel->pBirds[3].position.lig = DIMENSION - 2;
    plevel->pBirds[3].position.col = DIMENSION - 2;

    // Positionner la balle aleatoirement
    plevel->pBall->position.lig = rand() % (DIMENSION - 2) + 1;
    plevel->pBall->position.col = rand() % (DIMENSION - 2) + 1;
}

// SetUp Level with all elements
void initializeLevel(Level *plevel, int levelNumber)
{
    srand((unsigned int)time(NULL));
    initializeLevelAllocation(plevel, levelNumber);
    initializeParams(plevel);
    initializeSymbols(plevel);
    initializePos(plevel);

    // Initialize the grid
    for (int i = 0; i < DIMENSION; ++i)
    {
        for (int j = 0; j < DIMENSION; ++j)
        {

            plevel->gridMap[i][j] = ' ';

            if (i == 0 || i == DIMENSION - 1)
                plevel->gridMap[i][j] = '+';

            if (j == 0 || j == DIMENSION - 1)
                plevel->gridMap[i][j] = '+';
        }
    }

    // Place Walls and Traps Based on Level
    switch (levelNumber)
    {
    case 1:
        for (int i = 0; i < LEVEL1_WALLS; i++)
        {

            // Place Walls randomly, avoiding corners
            int wallRow, wallCol;
            do
            {
                wallRow = rand() % (DIMENSION - 2) + 1;         // Avoiding the first and last rows
                wallCol = rand() % (DIMENSION - 2) + 1;         // Avoiding the first and last columns
            } while (plevel->gridMap[wallRow][wallCol] != ' '); // Keep trying until an empty cell is found

            plevel->pWalls[i].position.lig = wallRow;
            plevel->pWalls[i].position.col = wallCol;

            plevel->gridMap[wallRow][wallCol] = plevel->pWalls->symbol;
        }

        for (int i = 0; i < LEVEL1_TRAPS; i++)
        {
            // Place Traps randomly, avoiding corners
            int trapRow, trapCol;
            do
            {
                trapRow = rand() % (DIMENSION - 2) + 1;         // Avoiding the first and last rows
                trapCol = rand() % (DIMENSION - 2) + 1;         // Avoiding the first and last columns
            } while (plevel->gridMap[trapRow][trapCol] != ' '); // Keep trying until an empty cell is found

            plevel->pTraps[i].position.lig = trapRow;
            plevel->pTraps[i].position.col = trapCol;

            plevel->gridMap[trapRow][trapCol] = plevel->pTraps->symbol;
        }
        break;
    }

    plevel->gridMap[plevel->pBall->position.lig][plevel->pBall->position.col] = plevel->pBall->symbol;

    // Place Snoopy
    plevel->gridMap[plevel->pSnoopy->position.lig][plevel->pSnoopy->position.col] = plevel->pSnoopy->symbol;

    // Place Birds
    for (int i = 0; i < 4; i++)
    {
        plevel->gridMap[plevel->pBirds[i].position.lig][plevel->pBirds[i].position.col] = plevel->pBirds[i].symbol;
    }
}

// ------------------END INITIALIZATION----------------------

// ------------------START EVENTLISTENING----------------------

char listenForKeyboardInput()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}
// ------------------END EVENTLISTENING----------------------

// ------------------START MOVEMENTS----------------------

int moveSnoopy(Level *plevel, char direction)
{
    moveBallDiagonally(plevel);
    int newLig = plevel->pSnoopy->position.lig;
    int newCol = plevel->pSnoopy->position.col;

    switch (direction)
    {
    case 'w':
        newLig--;
        break;
    case 's':
        newLig++;
        break;
    case 'a':
        newCol--;
        break;
    case 'd':
        newCol++;
        break;
    default:
        break;
    }

    // Check if the new position is within the bounds of the grid
    if (newLig >= 0 && newLig < DIMENSION && newCol >= 0 && newCol < DIMENSION)
    {
        // Check if the new position is an empty cell (you can modify this condition based on your game logic)
        if (plevel->gridMap[newLig][newCol] == ' ')
        {
            // Move Snoopy to the new position
            plevel->gridMap[plevel->pSnoopy->position.lig][plevel->pSnoopy->position.col] = ' '; // Clear the current position
            plevel->pSnoopy->position.lig = newLig;
            plevel->pSnoopy->position.col = newCol;
            plevel->gridMap[newLig][newCol] = plevel->pSnoopy->symbol; // Set Snoopy in the new position
        }
        else if (plevel->gridMap[newLig][newCol] == '*')
        {
            // Move Snoopy to the new position
            plevel->gridMap[plevel->pSnoopy->position.lig][plevel->pSnoopy->position.col] = ' '; // Clear the current position
            plevel->pSnoopy->position.lig = newLig;
            plevel->pSnoopy->position.col = newCol;
            plevel->gridMap[newLig][newCol] = plevel->pSnoopy->symbol; // Set Snoopy in the new position
            plevel->pSnoopy->lives--;
            if (plevel->pSnoopy->lives == 0)
                return -1;
        }
        else if (plevel->gridMap[newLig][newCol] == 'o')
        {
            // Move Snoopy to the new position
            plevel->gridMap[plevel->pSnoopy->position.lig][plevel->pSnoopy->position.col] = ' '; // Clear the current position
            plevel->pSnoopy->position.lig = newLig;
            plevel->pSnoopy->position.col = newCol;
            plevel->gridMap[newLig][newCol] = plevel->pSnoopy->symbol; // Set Snoopy in the new position
            plevel->pSnoopy->lives--;
            if (plevel->pSnoopy->lives == 0)
                return -1;
        }
        else if (plevel->gridMap[newLig][newCol] == '^')
        {
            // Move Snoopy to the new position
            plevel->gridMap[plevel->pSnoopy->position.lig][plevel->pSnoopy->position.col] = ' '; // Clear the current position
            plevel->pSnoopy->position.lig = newLig;
            plevel->pSnoopy->position.col = newCol;
            plevel->gridMap[newLig][newCol] = plevel->pSnoopy->symbol; // Set Snoopy in the new position
            plevel->score++;

            if (plevel->score == 4)
                return 1;
        }
    }
    return 3;
}

void moveBallDiagonally(Level *plevel)
{
    // Level *plevel = (Level *)arg;
    // Save the current position
    int currentLig = plevel->pBall->position.lig;
    int currentCol = plevel->pBall->position.col;

    // Move the Ball diagonally
    plevel->gridMap[currentLig][currentCol] = ' ';

    // Calculate the next position
    int nextLig = currentLig + directionRow;
    int nextCol = currentCol + directionCol;

    // Check if the next position is within bounds
    if (nextLig >= 0 && nextLig < DIMENSION && nextCol >= 0 && nextCol < DIMENSION)
    {
        // Check if the next position is an empty cell
        if (plevel->gridMap[nextLig][nextCol] == ' ')
        {
            // Move the ball to the next position
            plevel->pBall->position.lig = nextLig;
            plevel->pBall->position.col = nextCol;
        }
        else
        {
            // Reverse the direction of the ball if it reaches the boundaries
            directionRow = -directionRow;
            directionCol = -directionCol;
        }
    }
    else
    {
        // Reverse the direction of the ball if it reaches the boundaries
        directionRow = -directionRow;
        directionCol = -directionCol;
    }

    // Update the grid with the ball's new position
    plevel->gridMap[plevel->pBall->position.lig][plevel->pBall->position.col] = plevel->pBall->symbol;
}

// ------------------END MOVEMENTS----------------------

void displayGrid(Level *pLevel)
{
    system("clear"); // Clear the console screen
    for (int i = 0; i < DIMENSION; ++i)
    {
        for (int j = 0; j < DIMENSION; ++j)
        {
            printf("%c ", pLevel->gridMap[i][j]);
        }
        printf("\n");
    }

    printf("lives : %d, score : %d", pLevel->pSnoopy->lives, pLevel->score);
}
