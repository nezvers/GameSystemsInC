#include "raylib.h"
#include "TileMap.h"

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
    myMap.Populate();   //For testing purposes fill the map

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
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
