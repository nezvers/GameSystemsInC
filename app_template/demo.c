

#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

void init() {
    InitAudioDevice();
}

void finit() {
    CloseAudioDevice();
}

void update() {

}

void draw() {

}

int main(void)
{
    
    InitWindow(screenWidth, screenHeight, "App Template");
    SetTargetFPS(60);

    init();

    // Main game loop
    while (!WindowShouldClose())
    {
        update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();

        EndDrawing();
    }

    finit();
    CloseWindow();        // Close window and OpenGL context

    return 0;
}