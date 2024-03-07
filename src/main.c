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

#include "raylib.h"

#define numSprites 2

int main(void)
{
    //const int numSprites = 2;
    const int numFrames = 16;
    int screenWidth = 1280;
    int screenHeight = 720;
    int offset[numSprites] = {0, 1};
    int timeBuffer[2];

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    Texture2D puyo = LoadTexture("res/puyo/Aqua.png");
    Rectangle source = {.x = 1.0f, .y = 1.0f, .width = 32.0f, .height = 32.0f};
    Rectangle dest = {.x = 0, .y = source.height, .width = source.width, .height = source.height};

    timeBuffer[0] = (int)GetTime();
    timeBuffer[1] = timeBuffer[0];

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // run the code inside the if statement once every second
        timeBuffer[0] = (int)GetTime();
        if(timeBuffer[0] != timeBuffer[1])
        {
            timeBuffer[1] = timeBuffer[0];
            // increment every number in offset, making sure to roll back the index
            for(int i = 0; i < numSprites; ++i)
            {
                offset[i] += 1;
                if(offset[i] > numFrames)
                {
                    offset[i] = 0;
                }
            }
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        Rectangle source0 = source;
        Rectangle source1 = source;
        source0.x = (32 * offset[0]) + 1;
        source1.x = (32 * offset[1]) + 1;
        DrawTexturePro(puyo, source0, (Rectangle) {.x = 0, .y = 0, .width = 32, .height = 32}, (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(puyo, source1, (Rectangle) {.x = 31, .y = 0, .width = 32, .height = 32}, (Vector2){0, 0}, 0, WHITE);

        EndDrawing();
    }


    UnloadTexture(puyo);
    CloseWindow();        // Close window and OpenGL context


    return 0;
}
