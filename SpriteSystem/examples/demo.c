

#include "raylib.h"

#define SPRITE_IMPLEMENTATION
#include "sprite.h"
#include "sprite_raylib.h"

enum PLAYER_STATE {
    PlayerStateIdle,
    PlayerStateWalk,
    PlayerStateJumpUp,
    PlayerStateJumpDown,
};

const vec2 SPRITE_SIZE = {16, 16};
vec2 tex_pos[] = {{0.f,0.f}, {16.f,0.f}, {32.f,0.f}, {48.f,0.f}, {64.f,0.f}, {80.f,0.f}, {96.f,0.f}, {112.f,0.f}};
Frames anim_idle = {.data = &tex_pos[0], .count = 2, .size = SPRITE_SIZE};
Frames anim_walk = {.data = &tex_pos[2], .count = 6, .size = SPRITE_SIZE};
Frames anim_up =   {.data = &tex_pos[5], .count = 1, .size = SPRITE_SIZE};
Frames anim_down = {.data = &tex_pos[4], .count = 1, .size = SPRITE_SIZE};
Frames *player_anim_list[] = {&anim_idle, &anim_walk, &anim_up, &anim_down};

const AnimationSet player_animations = {
    .frames = player_anim_list,
    .count = 4, 
    .animation_index = PlayerStateIdle, 
    .image_index = 0, 
    .frame_rate = 12, 
    .time = 0,
};

Sprite player_sprite = (Sprite){
        .animation_set = player_animations,
        .position = {18, 60},
        .origin = {8, 16},
        .offset = {0, 0},
        .scale = {1, 1},
        .rotation = 0,
    };
Texture2D player_texture;

void init() {
    ChangeAnimation(&player_sprite.animation_set, PlayerStateWalk);

    player_texture = LoadTexture(ASSETS_PATH"/player_sheet.png");
}

void finit() {
    UnloadTexture(player_texture);
}

void update() {
    UpdateSprite(&player_sprite, GetFrameTime());
}

void draw() {
    Camera2D camera = { {0,0}, {0,0}, 0, 4};
    BeginMode2D(camera);

    rectf frame_rect = GetAnimationFrame(&player_sprite.animation_set);
    DrawTextureRec(player_texture, *(Rectangle*)&frame_rect, (Vector2){10,10}, RAYWHITE);
    EndMode2D();
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
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