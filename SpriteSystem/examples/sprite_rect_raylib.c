

#include "raylib.h"

#define NEZ_SPRITE_RECT_IMPLEMENTATION
#define NEZ_VEC2_F Vector2
#define NEZ_RECT_F Rectangle
#include "../sprite_rect.h"

Texture2D plumber_texture = { 0 };
Sprite plumber_sprite_1;
Sprite plumber_sprite_2;
SpriteAnimation plumber_animations[2];

// Array of texture offsets used for Sprite
Vector2 plumber_images[] = { {0.0f,0.0f}, { 16.0f,0.0f }, { 32.0f,0.0f }, { 48.0f,0.0f }, { 64.0f,0.0f }, { 80.0f,0.0f }, { 96.0f,0.0f } };

// Array of texture offset IDs for each animation
int plumber_idle_IDs[] = { 0 };
int plumber_walk_IDs[] = { 1,2,3,4,5,6 };


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    // Initialization
    plumber_texture = LoadTexture(SOURCE_PATH"/resources/player_sheet.png");
    plumber_animations[0] = SpriteAnimationCreate(1, 12, plumber_idle_IDs);
    plumber_animations[1] = SpriteAnimationCreate(6, 12, plumber_walk_IDs);
    plumber_sprite_1 = SpriteCreate(16, 16, -8, -16, 7, plumber_images); // Sets configurations
    plumber_sprite_2 = plumber_sprite_1; // Copy configuration

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // visualize origin position
        DrawLine(200, 170, 200, 230, PINK);
        DrawLine(170, 200, 230, 200, PINK);

        // reusable default values
        Vector2 origin = (Vector2){ 0.0f,  0.0f };
        float rotation = 0.0f;

        // for demonstration set values
        plumber_sprite_1.y = 200;
        plumber_sprite_2.y = 200;

        // Used for animation end detection
        bool is_finished = false;

        plumber_sprite_1.x = 200;
        plumber_sprite_1.x_scale = 2.f;
        plumber_sprite_1.y_scale = 2.f;
        // plays walking animation
        SpritePlay(&plumber_sprite_1, &plumber_animations[1], GetFrameTime(), true, &is_finished);
        DrawRectangleLinesEx(plumber_sprite_1.spr_rect, 1.f, BLACK);
        DrawTexturePro(plumber_texture, plumber_sprite_1.img_rect, plumber_sprite_1.spr_rect, origin, rotation, WHITE);

        //plays idle animation
        plumber_sprite_2.x = 200;
        plumber_sprite_2.x_scale = -4.f;
        plumber_sprite_2.y_scale = -4.f;
        SpritePlay(&plumber_sprite_2, &plumber_animations[0], GetFrameTime(), true, &is_finished);
        DrawRectangleLinesEx(plumber_sprite_2.spr_rect, 1.f, BLACK);
        DrawTexturePro(plumber_texture, plumber_sprite_2.img_rect, plumber_sprite_2.spr_rect, origin, rotation, WHITE);


        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(plumber_texture);

    CloseWindow();        // Close window and OpenGL context

    return 0;
}