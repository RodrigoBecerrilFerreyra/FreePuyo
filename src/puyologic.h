#ifndef PUYOLOGIC_H_
#define PUYOLOGIC_H_

#include <stdbool.h>

/*
    Sprite states:
    0  - not connected
    1  - connected below
    2  - above
    3  - above and below
    4  - right
    5  - right and below
    6  - right and above
    7  - right, above, and below
    8  - left
    9  - left and below
    10 - left and above
    11 - left, above, and below
    12 - left and right
    13 - left, right, and below
    14 - left, right, and above
    15 - all
*/

typedef struct Puyo
{
    unsigned char color; // the color of the puyo in one letter, e.g. 'r' for red
    bool connectedTop;
    bool connectedBot;
    bool connectedLef;
    bool connectedRig;
    unsigned int numConnections;
} Puyo;

typedef struct PuyoBoard
{
    unsigned int rows;
    unsigned int columns;
    unsigned int hiddenRows;
    unsigned int sizeOfBoard;
    Puyo **slots; // dynamically allocated array holding pointers to all the puyo
    // starts at the top left of the board and goes left to right, top to bottom including hidden rows
    // the size of slots will be sizeof(Puyo *) * rows * columns
    int *buffer; // dynamically allocated array of indeces of size sizeof(int) * rows * columns
} PuyoBoard;

// board related functions
PuyoBoard *initializeBoard(unsigned int rows, unsigned int columns, unsigned int hiddenrows);
void destroyBoard(PuyoBoard *board);
void destroyAllPuyo(PuyoBoard *board);
void clearBuffer(PuyoBoard *board);

// puyo related functions
//void checkForPairs(PuyoBoard, unsigned int);
int createPuyo(PuyoBoard *board, unsigned char color, unsigned int index);
void destroyPuyo(PuyoBoard *board, unsigned int index);
void dropPuyo(PuyoBoard *board);


#endif //PUYOLOGIC_H_
