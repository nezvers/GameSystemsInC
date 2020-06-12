#include "raylib.h"
#include "TileMap.h"
#include <iostream>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 320;
    const int screenHeight = 180;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    //LOAD THERE
    //Texture2D tex = LoadTexture("resources/TileSetter_template.png");
    //Vector2 center = Vector2{float(screenWidth/2), float(screenHeight/2)};

    //Inside main
    TileSet myTiles("resources/TileSetter_template.png", {16.0f, 16.0f});
    TileMap myMap(myTiles, {64.0f, 64.0f}, 10, 5);
    int index = 33;
    myMap.Populate();   //For testing purposes fill the map

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) myMap.SetTileWorld(GetMousePosition(), index);
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) myMap.SetTileWorld(GetMousePosition(), -1);
        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
            int i = myMap.GetTileWorld(GetMousePosition());
            index = i > -1 ? i : index;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            //myTiles.DrawTile(13, center);
            myMap.Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
