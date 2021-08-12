

#include "raylib.h"
#include "math.h"

#define HITBOX_COLLISIONS_IMPLEMENTATION
#include "../hitbox_collisions.h"

const int screenWidth = 800;
const int screenHeight = 600;

void GameInit();
void GameUpdate();
void GameDraw();

#define SOLID_COUNT 4
Hitbox solids[] = {
    {200.0f, 200.0f, 16.0f, 16.0f},
    {216.0f, 184.0f, 16.0f, 16.0f},
    {224.0f, 168.0f, 16.0f, 16.0f},
    {232.0f, 152.0f, 16.0f, 16.0f},
};
Hitbox player = {100.0f, 100.0f, 16.0, 16.0f};

float spdX = 0.0f;
float spdY = 0.0f;

int main(void)
{
    GameInit();
    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        GameUpdate();
        GameDraw();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void GameInit() {
    InitWindow(screenWidth, screenHeight, "raylib [models] example - geometric shapes");
}


void GameUpdate() {
    spdX = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * 2.0f;
    spdY = (float)(IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * 2.0f;
    for (int i = 0; i < SOLID_COUNT; i++) {
        HitboxMoveAndCollideHitbox(&player, &solids[i], &spdX, &spdY);
    }
    player.x += spdX;
    player.y += spdY;
}


void GameDraw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle((int)player.x, (int)player.y, (int)player.w, (int)player.h, BLUE);
    for (int i = 0; i < SOLID_COUNT; i++) {
        DrawRectangle((int)solids[i].x, (int)solids[i].y, (int)solids[i].w, (int)solids[i].h, RED);
    }
    

    EndDrawing();
}
