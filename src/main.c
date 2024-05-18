/*
    Copyright (C) 2023 Rodrigo Becerril Ferreyra

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <stdio.h>
#include "raylib.h"
#include "puyologic.h"

int main(void)
{
    int errorcode;
    int screenWidth = 1280;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    Texture2D puyo = LoadTexture("res/puyo/Aqua.png");
    Rectangle source = {.x = 1.0f, .y = 1.0f, .width = 31.0f, .height = 31.0f};
    //Rectangle dest = {.x = 0, .y = source.height, .width = source.width, .height = source.height};

    PuyoBoard *board = initializeBoard(12, 6, 1);
    if(board == NULL)
    {
        return 1;
    }

    errorcode = createPuyo(board, 72, 'r');
    if(errorcode != 0)
    {
        printf("Error code: %d\n", errorcode);
        return errorcode;
    }

    errorcode = createPuyo(board, 0, 'r');
    if(errorcode != 0)
    {
        printf("Error code: %d\n", errorcode);
        return errorcode;
    }

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //DrawTexturePro(puyo, source, indexToRect(board, ), (Vector2){0, 0}, 0, WHITE);
        //DrawTexturePro(puyo, source1, (Rectangle) {.x = 31, .y = 0, .width = 32, .height = 32}, (Vector2){0, 0}, 0, WHITE);

        // draw every single puyo
        for(unsigned int slot = 0; slot < board->sizeOfBoard; ++slot)
        {
            Rectangle dest = indexToRect(board, slot);
            if(board->slots[slot] != NULL)
            //if(1)
            {
                DrawTexturePro(puyo, source, dest, (Vector2){0, 0}, 0, WHITE);
            }
        }

        EndDrawing();
        dropPuyo(board);
    }


    UnloadTexture(puyo);
    CloseWindow();        // Close window and OpenGL context


    return 0;
}
