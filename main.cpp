#include "raylib.h"
#include "raymath.h"
#include "TileMap.h"
#include <iostream>

const int screenWidth = 320*2;
const int screenHeight = 180*2;

void UpdateMap(TileMap& map, int& index, Camera2D& camera){               //For Testing and demonstration purpose
    Vector2 worldPos = Vector2Scale(Vector2Subtract(GetMousePosition(), camera.offset), camera.zoom*0.25);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))        map.SetTileWorld(worldPos, index);
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))       map.SetTileWorld(worldPos, -1);
    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
        int i = map.GetTileWorld(worldPos);
        index = i > -1 ? i : index;
        std::cout << i << std::endl;;
    }
}

void MoveCamera(Camera2D& camera){
    camera.offset.x -= (float)IsKeyDown(KEY_RIGHT) - (float)IsKeyDown(KEY_LEFT);
    camera.offset.y -= (float)IsKeyDown(KEY_DOWN) - (float)IsKeyDown(KEY_UP);
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    TileSet myTiles("resources/TileSetter_template.png", {16.0f, 16.0f});
    TileMap myMap(myTiles, {64.0f, 64.0f}, 10, 5);

    //For testing
    int index = 33;
    myMap.Populate();   //For testing purposes fill the map

    Camera2D camera = { 0 };
    camera.target = (Vector2){0.0, 0.0};
    camera.offset = (Vector2){0.0, 0.0};
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        UpdateMap(myMap, index, camera);
        MoveCamera(camera);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);

                myMap.Draw();

            EndMode2D();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
