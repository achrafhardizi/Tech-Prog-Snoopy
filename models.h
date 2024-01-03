#ifndef MODELS_H
#define MODELS_H

#define DIMENSION 13
#define TIME 60
#define LIVES 3

#define LEVEL1_WALLS 5
#define LEVEL1_TRAPS 3

#define LEVEL2_WALLS 10
#define LEVEL2_TRAPS 6

#define LEVEL3_WALLS 15
#define LEVEL3_TRAPS 9

typedef struct
{
    int col;
    int lig;
} Position;

typedef struct
{
    Position position;
    char symbol;
    int lives;
} Snoopy;

typedef struct
{
    Position position;
    char symbol;
} Ball;

typedef struct
{
    Position position;
    char symbol;
} Bird;

typedef struct
{
    Position position;
    char symbol;
} Wall;

typedef struct
{
    Position position;
    char symbol;
} Trap;

typedef struct
{
    Snoopy *pSnoopy;
    Ball *pBall;
    Bird pBirds[4];
    Wall *pWalls;
    Trap *pTraps;
    char gridMap[DIMENSION][DIMENSION];
    int time;
    int score;
} Level;

// SetUp Level Allocation for Different Elemnts
void initializeLevelAllocation(Level *, int);

// SetUp Params
void initializeParams(Level *);

// SetUp Symbols
void initializeSymbols(Level *);

// SetUp Positions;
void initializePos(Level *);

// SetUp Level;
void initializeLevel(Level *, int);

// Event listening functions
char listenForKeyboardInput();

// Movement functions
int moveSnoopy(Level *, char);

void moveBallDiagonally(Level *);

// Display function
void displayGrid(Level *);

#endif // MODELS_H
