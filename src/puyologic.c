#include <stdlib.h>
#include "puyologic.h"

 PuyoBoard *initializeBoard(unsigned int rows, unsigned int columns, unsigned int hiddenrows)
 {
    PuyoBoard *board = malloc(sizeof(PuyoBoard));
    if(board == NULL)
    {
        TraceLog(LOG_FATAL, "Unable to allocate memory for PuyoBoard.\n");
        return board;
    }

    board->rows = rows;
    board->columns = columns;
    board->hiddenRows = hiddenrows;
    board->slots = malloc(sizeof(Puyo) * rows * columns);
    if(board->slots == NULL)
    {
        TraceLog(LOG_FATAL, "Unable to allocate memory for slots in PuyoBoard.\n");
        return board;
    }

    return board;
 }

 void destroyBoard(PuyoBoard board)
 {
     free(board->slots);
     free(board);
 }

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
