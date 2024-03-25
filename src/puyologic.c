#include <stdlib.h>
#include "raylib.h"
#include "puyologic.h"

PuyoBoard *initializeBoard(unsigned int rows, unsigned int columns, unsigned int hiddenrows)
{
    // allocate space for board
    PuyoBoard *board = malloc(sizeof(PuyoBoard));
    if(board == NULL)
    {
        TraceLog(LOG_FATAL, "Unable to allocate memory for PuyoBoard.\n");
        return NULL;
    }

    // fill in fields
    board->rows = rows;
    board->columns = columns;
    board->hiddenRows = hiddenrows;
    board->sizeOfBoard = (rows + hiddenrows) * columns;

    // allocate space for puyo pointer array
    board->slots = malloc(sizeof(Puyo *) * board->sizeOfBoard);
    if(board->slots == NULL)
    {
        TraceLog(LOG_FATAL, "Unable to allocate memory for slots in PuyoBoard.\n");
        return NULL;
    }

    // initialize puyo pointer array with null pointers
    for(unsigned int i = 0; i < board->sizeOfBoard; ++i)
    {
        board->slots[i] = NULL;
    }

    // allocate space for buffer
    board->buffer = malloc(sizeof(int) * board->sizeOfBoard);
    if(board->buffer == NULL)
    {
        TraceLog(LOG_FATAL, "Unable to allocate memory for buffer in PuyoBoard.\n");
        return NULL;
    }

    // initialize buffer with -1
    for(unsigned int i = 0; i < board->sizeOfBoard; ++i)
    {
        board->buffer[i] = -1;
    }

    return board;
}

void destroyBoard(PuyoBoard *board)
{
    free(board->slots);  board->slots = NULL;
    free(board->buffer); board->buffer = NULL;
    free(board);         board = NULL;
}

/*
    Main gameplay loop:
    1. drop all puyo floating in the air. Also take note of their indeces (final positions)
    2. check for connected puyo that moved. Set the respective connections. Update numConnections of puyo in question and respective puyo
    3. check for numConnections of 4 or more and pop them
    4. goto step 1
*/

int createPuyo(PuyoBoard *board, unsigned char color, unsigned int index)
{
    // check if index fits in the board
    if(index >= board->sizeOfBoard)
    {
        return 1;
    }
    // check if index is free
    if(board->slots[index] != NULL)
    {
        return 2;
    }

    // allocate space for puyo
    Puyo *puyo = malloc(sizeof(Puyo));
    if(puyo == NULL)
    {
        TraceLog(LOG_FATAL, "Unable to allocate memory for a Puyo.\n");
        return 3;
    }

    puyo->color = color;
    // insert puyo into index
    board->slots[index] = puyo;
    return 0;
}

void destroyPuyo(PuyoBoard *board, unsigned int index)
{
    Puyo *puyo = board->slots[index];
    free(puyo);
    puyo = NULL;
    board->slots[index] = NULL;
}

void dropPuyo(PuyoBoard *board)
{
    // go through each column and check for floating puyo
    // move all puyo above it down as many spaces as needed
    // check column again, repeat if necessary
    // move to the next column

    // always starts at the bottom of column 0
    unsigned int bottomOfColumn = board->rows + board->hiddenRows;
    for(unsigned int i = 0; i < board->columns; ++i)
    {
        // to move up in a row, subtract the amount of columns
    }
}

/*
void checkForPairs(PuyoBoard board, unsigned int index)
{
    // Checks the spaces around the selected puyo,
    // changes its animation sprite, and marks it as ready to pop.

    // step 1: mark current puyo as checked
    // step 2: take a list of adjacent puyo's indices. if puyo doesn't exist (puyo is just the wall) mark as -1
    // step 3: check adjacent puyo if they have not been checked yet
    //   if the adjacent puyo matches the color of the current puyo, take note of it
    // step 4: adjust the sprite state based on the adjacent puyo's color matches
    // step 5: call checkForPairs on the color matches
    // step 6: unmark current puyo as checked
}
*/
