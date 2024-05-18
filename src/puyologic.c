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
    clearBuffer(board);

    return board;
}

void destroyBoard(PuyoBoard *board)
{
    destroyAllPuyo(board);
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

int createPuyo(PuyoBoard *board, unsigned int index, unsigned char color)
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

void destroyAllPuyo(PuyoBoard *board)
{
    for(unsigned int puyo = 0; puyo < board->sizeOfBoard; ++puyo)
    {
        destroyPuyo(board, puyo);
    }
}

void clearBuffer(PuyoBoard *board)
{
    for(unsigned int i = 0; i < board->sizeOfBoard; ++i)
    {
        board->buffer[i] = -1;
    }
}

Rectangle indexToRect(PuyoBoard *board, unsigned int index)
{
    /**
    * Creates a Rectangle object corresponding to the index of the puyo.
    */
    unsigned int column;
    unsigned int row;

    row = index / board->columns;
    column = index % board->columns;
    row *= 31; column *= 31;
    return (Rectangle){.x = column, .y = row, .width = 31, .height = 31}; // 31 is the size of the sprite
}

bool dropPuyo(PuyoBoard *board)
{
    // go through each column and check for floating puyo
    // move all puyo above it down as many spaces as needed
    // check column again, repeat if necessary
    // move to the next column

    int emptySpace;
    unsigned int puyoToMove;

    const unsigned int numRows = board->rows + board->hiddenRows;

    // columnsTouched[n] is true if puyo were dropped, false otherwise
    bool columnsTouched[board->columns];
    for(unsigned int col = 0; col < board->columns; ++col)
    {
        columnsTouched[col] = false;
        emptySpace = -1;

        // always starts at the bottom of column 0
        int cursor = ((numRows - 1) * board->columns) + col;
        // to move up in a row, subtract the amount of columns
        // until you reach < 0 (off the board)

        /* step 1: start from the bottom of the column. If it's empty, no need to proceed
         * step 2: move up in the column. To move up in a column, subtract the amount of columns
         *   if no puyo are found, the loop exits and nothing is done
         * step 3: keep track of the puyo until there's an empty space. Take note of the index of the empty space.
         *   Then, keep track of the empty spaces until there's a puyo found.
         *   (if none are found (only empty spaces), refer to 2.5)
         * step 4: take note of the index the separated puyo. Move the Puyo into the empty space previously noted.
         *   Set columnsTouched to true. Move the index of the empty space one up (the next empty space available above).
         *   repeat this process for all puyo in the column going up until the cursor is off the board.
         */

        // step 1
        // TODO: Moving puyo down fails if the bottom slot is empty for any given column (e.g. if there's a Puyo on row 1 but not row 0)
        if(board->slots[cursor] == NULL) // if the column is empty
        {
            continue; // go to the next column
        }

        // step 2/3: look for the first free space and take note of it
        while(cursor > 0)
        {
            // move the cursor up one
            cursor -= board->columns;
            if(cursor < 0) // if the cursor goes off the board
            {
                break;
            }
            // else if slot is empty
            if (board->slots[cursor] == NULL)
            {
                if(emptySpace == -1) // only assigned the first time around for each column
                {
                    emptySpace = cursor;
                }
                break;
            } // else if it's not empty, run the loop again
        }

        // step 3/4: look for the next puyo to move down
        while(cursor > 0)
        {
            cursor -= board->columns;
            if(cursor < 0)
            {
                break;
            }
            if(board->slots[cursor] != NULL) // if a puyo is found (floating)
            {
                // mark the column as true
                columnsTouched[col] = true;
                // take note of puyo
                puyoToMove = cursor;
                // move the puyo to the empty slot
                board->slots[emptySpace] = board->slots[puyoToMove];
                board->slots[puyoToMove] = NULL;
                // move the empty space up one on the column
                // (this is guaranteed to be on the board)
                emptySpace -= board->columns;
            }
        } // keep doing this until the end of the column is reached
    }

    // check the results of columnsTouched
    for(unsigned int i = 0; i < board->columns; ++i)
    {
        if(columnsTouched[i]) // if true for any column
        {
            return true;
        }
    }
    // else
    return false;
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
