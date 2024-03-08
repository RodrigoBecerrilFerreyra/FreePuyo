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

 enum spriteState
 {
     NONE = 0,
     D = 1, U = 2, UD = 3,
     R = 4, RB = 5, RU = 6,  RUD = 7,
     L = 8, LB = 9, LU = 10, LUD = 11,
     LR = 12, LRB = 13, LRA = 14, LRUD = 15
 };

 typedef struct Puyo
 {
     unsigned char color; // the color of the puyo in one letter, e.g. 'r' for red
     enum spriteState state; // describes which of the 16 sprites are displayed
     bool readyToPop; // whether or not the puyo will be popped in the next pop check
     bool checked;
 } Puyo;

 typedef struct PuyoBoard
 {
     unsigned int rows;
     unsigned int columns;
     unsigned int hiddenRows;
     Puyo *slots; // dynamically allocated array holding all the puyo
     // starts at the top left of the board and goes left to right, top to bottom including hidden rows
     // the size of slots will be sizeof(Puyo) * rows * columns
 } PuyoBoard;

 PuyoBoard *initializeBoard(unsigned int rows, unsigned int columns, unsigned int hiddenrows);
 void destroyBoard(PuyoBoard);

 // puyo checking functions
 void checkForPairs(PuyoBoard, unsigned int);

 #endif //PUYOLOGIC_H_
