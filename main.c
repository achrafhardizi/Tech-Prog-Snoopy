#include "models.h"
#include <stdlib.h>
// #include <pthread.h>
#include <stdio.h>

int main()
{
    Level *level = (Level *)malloc(sizeof(Level));
    int currentLevel = 1;

    // Initialize the level
    initializeLevel(level, currentLevel);

    // Main game loop
    char key;
    int result = 3;

    // ball movement
    // pthread_t ballThread;

    do
    {

        // Display the new state of the grid
        displayGrid(level);

        // Listen for keyboard input
        key = listenForKeyboardInput();

        // Move Snoopy based on the input
        result = moveSnoopy(level, key);
        // pthread_create(&ballThread, NULL, moveBallDiagonally, (void *)level);

    } while (key != 'q' && result == 3); // Exit the loop when 'q' is pressed

    if (result == 1)
        printf("\nmbarek w ms3oud");
    if (result == -1)
        printf("\ndb hadi game ykhsro fiha nass ha l3ar");

    // Clean up allocated memory
    free(level->pSnoopy);
    free(level->pBall);

    // Depending on the level, free additional memory
    switch (currentLevel)
    {
    case 1:
        free(level->pWalls);
        free(level->pTraps);
        break;
    }

    free(level);
    return 0;
}
